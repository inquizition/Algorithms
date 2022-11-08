#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>
#include<time.h>
#include<error.h>

float dist[3];

typedef struct Layer {
    float *weights;
    float *bias;
    float *q;
    float *z;
    float *dQ;
    uint8_t M;
    uint8_t N;
    uint8_t inputSize;
    struct Layer *next;
} Layer;

float ReLu(float z) {
    if(z > 0) {
        return z;
    } else {
        return 0;
    }
}

float dReLu(float z) {
    if(z > 0) {
        return 1;
    } else {
        return 0;
    }
}

int16_t Q15(float num) {
    return num*pow(2,15);
}

float toQ15(int16_t num, int mult) {
    return ((num-((pow(2,16))/2))/pow(2,15))*mult;
}


void dispMatrix(float *mat, int M, int N) {
    printf("Matrix: \n");
    printf("[%f", mat[0]);
    int i;
    for(i = 1; i < M*N; i++) {
        if(i % N == 0) {
            printf("] \n");
        } else {
            printf(", ");
        }
        printf("%f", mat[i]);
    }
    printf("]\n \n");
}

float vecSum(float* vec1, float* vec2, float bias) {
    size_t n = sizeof(vec1)/sizeof(vec1[0]);
    float sum = 0;

    for(int i=0; i < n+1; i++) {
        sum += (vec1[i] * vec2[i]) + bias;
    }

    return sum;
}

void transpose(float *t, int M, int N, float *mat) {
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            mat[j + (N*i)] = t[(j*M) + i];
        }
    }
}

void mMult(float* m1, float* m2, int M, int N, float* mOut) {
    /* M[0] x N[1] matrix */
    /* M[0] rows, N[1] columns */

    for(int i = 0; i < N; i++) {
        mOut[i] = 0;
        for(int j = 0; j < M; j++) {
            mOut[i] += m1[j] * m2[(j*N) + i];
        }
    }
}

void mDot(float *m1, float *m2, int M, int N, float *mOut)  {
    /* M[0] rows, N[1] columns */
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            mOut[(i*N)+j] = 0;
            mOut[(i*N)+j] = m1[i] * m2[j];
        }
    }
}

void addC(float *v1, float c, int N, uint8_t relu) {

    if(relu == 0) {
        for(uint8_t i = 0; i <= N; i++) {
            v1[i] = v1[i] + c;
        }
    } else {
        for(uint8_t i = 0; i <= N; i++) {
            v1[i] = ReLu(v1[i] + c);
        }
    } 
}

void vecSub(float *v1, float *v2, int M, float lr) {
    for(int i = 0; i < M; i++) {
        v1[i] = v1[i] - (v2[i]*lr);
    }
}

void vecAdd(float *v1, float *v2, int M, float* z, uint8_t relu) {
    
    for(int i = 0; i < M; i++) {
        z[i] = v1[i] + v2[i];
    }
    if(relu == 0) {
        for(int i = 0; i < M; i++) {
            v1[i] = z[i];
        }
    } else {
        for(int i = 0; i < M; i++) {
            v1[i] = ReLu(z[i]);
        }
    } 
}

float sum(float *v, int M) {
    float sum = 0;
    for(int i = 0; i < M; i++) {
        sum += v[i];
    }
    return sum;
}

float softMax(float *input, float *output, uint8_t N) {
    
    float temp;

    for(int i = 0; i < N; i++) {
        output[i] = 0;
        output[i] = exp(input[i]);
        temp += output[i];
    }

    for(int i = 0; i < N; i++) {
        output[i] = output[i] / temp;
    }

    return temp;
}

float lossFunc(float *output, uint8_t y, uint8_t nrOfClasses) {

    float loss = 0;
    for(int i = 0; i < nrOfClasses; i++) {
        loss += exp(output[i]);
    }
    
    loss = log(loss) - output[y];
    //printf("Loss: %f \n", loss);
    if(loss > 10) {
        printf("Whut\n");
    }

    return loss;
}

static void reverse(struct Layer** head_ref) {
    struct Layer* prev = NULL;
    struct Layer* current = *head_ref;
    struct Layer* next = NULL;
    while (current != NULL) {
        // Store next
        next = current->next;
 
        // Reverse current node's pointer
        current->next = prev;
 
        // Move pointers one position ahead.
        prev = current;
        current = next;
    }
    *head_ref = prev;
}

int find_maximum(float *a, int n) {
  int c, index = 0;

  for (c = 1; c < n; c++)
    if (a[c] > a[index])
      index = c;

  return index;
}

void convolutionalLayer(Layer *l) {
    int M = l->next->M;
    int N = l->next->N;

    float *W = l->next->weights;
    uint16_t count = 0;

    for(int i = 0; i < (l->inputSize)-(M); i+=2) {
        l->next->q[count] = 0;
        for(int j = 0; j < M; j++) {
            l->next->q[count] += l->q[j + i]*l->next->weights[j];
        }
        l->next->z[count] = l->next->q[count] + l->next->bias[0];
        l->next->q[count] = ReLu(l->next->z[count]);
        count++;
    }
    l->next->inputSize = count;
}

void backConv(Layer *l, float lr, uint8_t stride) {
    int M = l->M;
    int N = l->N;
    float dW[N*M];
    float dZ[l->inputSize];
    float db = 0;

    float Wt[l->inputSize * l->next->inputSize];
    
    uint8_t count = 0;
    uint16_t lb = 0;
    uint16_t ub = lb + 4;
    
    for(int i = 0; i < l->inputSize; i++) {
        for(int j = 0; j < l->next->inputSize; j++) {
            if(j >= lb && j < ub) {
                Wt[j + (i*l->next->inputSize)] = l->weights[count];
                count++;
            } else {
                Wt[j + (i*l->next->inputSize)] = 0;
                count = 0;
            }
        }
        lb += 2;
        ub += 2;
    }

    for(int i = 0; i < l->inputSize; i++) {
        dZ[i] = l->dQ[i] * dReLu(l->z[i]);
        db +=dZ[i];
    }

    mMult(dZ, Wt, l->inputSize, l->next->inputSize, l->next->dQ);

    for(int i = 0; i < M; i+=2) {
        dW[i] = 0;
        for(int j = 0; j < l->inputSize; j++) {
            dW[i] += dZ[j] * l->next->q[j + i];
        }
    }

    l->bias[0] -= db*lr;
    vecSub(l->weights, dW, N*M, lr);
}

void backDense(Layer *l, float *dZ, float lr, uint8_t firstLayer) {
    int M = l->M;
    int N = l->N;
    float dW[N*M];

    if(!firstLayer) {
        for(int i = 0; i < N; i++) {
            dZ[i] = l->dQ[i] * dReLu(l->z[i]);
        }
    }

    mMult(dZ, l->weights, N, M, l->next->dQ);
    mDot(dZ, l->next->q, N, M, dW);

    vecSub(l->bias, dZ, N, lr);
    vecSub(l->weights, dW, N*M, lr);
}

uint8_t denseLayer(Layer *l, uint8_t activation) {
    int M = l->next->M;
    int N = l->next->N;
    float W[M*N];

    transpose(l->next->weights, M, N, W);
    mMult(l->q, W, M, N, l->next->q);
    vecAdd(l->next->q, l->next->bias, N, l->next->z, activation);

    return N;
}


void backwardPass(Layer *l, float *dZ, float lr) {
    Layer *lit = l;
    //printf("Layer 4 M: %d, N: %d \n", lit->M, lit->N);
    backDense(lit, dZ, lr, 1);
    //dispMatrix(lit->weights, 5,3);
    lit = lit->next;
    //printf("Layer 3 M: %d, N: %d \n", lit->M, lit->N);
    backDense(lit, dZ, lr, 0);
    //dispMatrix(lit->weights, 17,5);
    lit = lit->next;
    //printf("Layer 2 M: %d, N: %d \n", lit->M, lit->N);
    backConv(lit, lr, 2);
    //dispMatrix(lit->weights, 4,1);
    lit = lit->next;
    //printf("Layer 1 M: %d, N: %d \n", lit->M, lit->N);
    backConv(lit, lr, 2);
    //dispMatrix(lit->weights, 4,1);

    reverse(&l);
}

void forwardPass(Layer *l, float *y_hat, uint8_t ye, float lr) {

    convolutionalLayer(l);
    //dispMatrix(l->q, 10,1);
    //printf("Layer 1 M: %d, N: %d \n", l->M, l->N);
    Layer *lit = l->next;
    //printf("Layer 2 M: %d, N: %d \n", lit->M, lit->N);
    convolutionalLayer(lit);
    lit = lit->next;
    //printf("Layer 3 M: %d, N: %d \n", lit->M, lit->N);
    denseLayer(lit, 1);
    lit = lit->next;
    //printf("Layer 4 M: %d, N: %d \n", lit->M, lit->N);
    uint8_t N = denseLayer(lit, 0);
    lit = lit->next;
    //printf("Layer 5 M: %d, N: %d \n", lit->M, lit->N);

    /* Calculate loss and softmax */
    float loss = lossFunc(lit->q, ye, N);
    float ei = softMax(lit->q, y_hat, N);

    float dZ[N];
    for(uint8_t i = 0; i < N; i++) {
        dZ[i]= -(ye == i) + y_hat[i];
    } 

    reverse(&l);
    backwardPass(l,dZ,lr);
}

float validate(Layer *l, float *dist, int y) {

    convolutionalLayer(l);
    //printf("Layer 1 M: %d, N: %d \n", l->M, l->N);
    Layer *lit = l->next;
    //printf("Layer 2 M: %d, N: %d \n", lit->M, lit->N);
    convolutionalLayer(lit);
    lit = lit->next;
    //printf("Layer 3 M: %d, N: %d \n", lit->M, lit->N);
    denseLayer(lit, 1);
    lit = lit->next;
    //printf("Layer 4 M: %d, N: %d \n", lit->M, lit->N);
    uint8_t N = denseLayer(lit, 0);
    lit = lit->next;
    //printf("Layer 5 M: %d, N: %d \n", lit->M, lit->N);

    /* Calculate loss and softmax */
    float loss = lossFunc(lit->q, y, N);
    float ei = softMax(lit->q, dist, N);

    return loss;
}

int predict(Layer *l) {
    
    convolutionalLayer(l);

    Layer *lit = l->next;
    convolutionalLayer(lit);
    lit = lit->next;

    denseLayer(lit, 1);
    lit = lit->next;
    
    uint8_t N = denseLayer(lit, 0);
    lit = lit->next;

    /* Calculate loss and softmax */
    int guess = -1;
    softMax(lit->q, dist, N);
    guess = find_maximum(dist, N);

    return guess;
}

void initWeights(Layer *l) {
    /* M[0] rows, N[1] columns */
    Layer *current = l;
    while(current != NULL) {
        for(int i=0; i < (current->M)*(current->N); i++) {
            if(i < current->N) {
                current->bias[i] = ((float)(rand() % 100)/100);
            }
            current->weights[i] = ((float)(rand() % 100)/100) - 0.5;
        }
        current = current->next;
    }
    
}