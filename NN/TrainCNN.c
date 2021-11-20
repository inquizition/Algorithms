#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>
#include<time.h>
#include<error.h>
#include"CNN.c"

float x2[80];

#define DATALEN         800

uint8_t y[3] = {0,1,2};

struct my_record {
    int xa;
    int xb;
};

struct my_data{
        float x[80];
        uint8_t y;
    };

struct my_data2{
        float x[DATALEN];
        uint8_t y;
    };

void readData(struct my_data *data, int16_t numOfData) {
    FILE* fp;
    struct my_record records[100];
    size_t count = 0;
    char class;
    int class_num;
    char *addresses;

    for(int m = 0; m < 3; m++) {

        if(m == 0) {
            addresses = "C:/Users/rytte/OneDrive/Dokument/GitHub/Exjobb/O-symbols.csv";
        } else if (m == 1) {
            addresses = "C:/Users/rytte/OneDrive/Dokument/GitHub/Exjobb/V-symbols.csv";
        } else if (m == 2) {
            addresses = "C:/Users/rytte/OneDrive/Dokument/GitHub/Exjobb/line-symbols.csv";
        }

        errno = 0;

        fp = fopen(addresses, "r");
        
        if (fp == NULL) {
            printf("Error %d \n", errno);
            fprintf(stderr, "Error reading file\n");
        }

        for(int k = 0; k < numOfData; k++) {
        
            size_t count = 0;
            if(m == 3) {
                fscanf(fp, " %c,", &class);
            } else {
                fscanf(fp, " %c", &class);
            }
            

            data[k + (m*numOfData)].y = m;

            while (fscanf(fp, " %d,%d", &records[count].xa, &records[count].xb) == 2) {
                count++;
            }

            for (size_t i = 0; i < count; i++) {
                data[k + (m*numOfData)].x[i*2] = 0;
                data[k + (m*numOfData)].x[(i*2)+1] = 0;
                data[k + (m*numOfData)].x[i*2] = (float)records[i].xa/10000;
                data[k + (m*numOfData)].x[(i*2)+1] = (float)records[i].xb/10000;
            }
        }
        fclose(fp);
    }
    
}

void writeData(struct Layer *l, float loss, float acc, const char* fname) {
    FILE *fp;

    fp = fopen(fname, "w+");
    int count = 0;
    fprintf(fp, "Model: Loss = %f Acc = %f \n", loss, acc);
    while(l != NULL) {
        fprintf(fp, "w %d: M = %d N = %d\n", count, l->M, l->N);
        for(int i = 0; i < l->M*l->N; i++) {
            fprintf(fp, "%f\n", l->weights[i]);
        }
        fprintf(fp, "Bias: \n");
        for(int i = 0; i < l->N; i++) {
            fprintf(fp, "%f\n", l->bias[i]);
        }
        count++;
        l = l->next;
    }

    fclose(fp);
    
}

void shuffle(struct my_data *data, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          struct my_data t = data[j];
          data[j] = data[i];
          data[i] = t;
        }
    }
}

void train(struct my_data *data, uint16_t nOfData, uint16_t epochs, float lr, size_t inpSize) {
    srand(time(NULL));

    size_t filtSize = 2;

    /* Conv layer sizes */
    size_t s1 = (int) ((inpSize - (filtSize*2))/filtSize);

    size_t s2 = (int) ((s1 - (filtSize*2))/filtSize);

    /* Dense layer sizes */
    size_t s3 = 5;
    size_t s4 = 3;

    /* Filter 2 Sizes */
    float weights2[s2][s3];
    float weights3[s3][s4];

    float convW1[filtSize][filtSize];
    float convW2[filtSize][filtSize];

    float b1[1];
    float b2[1];
    float b3[s3];
    float b4[s4];
    
    int batches = 10;
    uint16_t val_size = floor(nOfData/batches);
    uint16_t training_size = nOfData-(2*val_size);

    struct my_data val[val_size];

    struct my_data hold_out[val_size];
    struct my_data tra[training_size];

    //int inputSize = 80;

    float x1[inpSize];
    
    float l2q[s1];
    float l3q[s2];
    float l4q[s3];
    float l5q[s4];

    float z2[s1];
    float z3[s2];
    float z4[s3];
    float z5[s4];

    float dQ4[s3];
    float dQ3[s2];
    float dQ2[s1];
    float dQ1[inpSize];
    
    Layer *fLayer = NULL;
    fLayer = (Layer *) malloc(sizeof(Layer));
    Layer *l1 = fLayer;
    l1->q = x1;
    l1->M = inpSize;
    l1->inputSize = inpSize;
    l1->dQ = dQ1;

    l1->next = (Layer *) malloc(sizeof(Layer));
    l1 = l1->next;
    l1->q = l2q;
    l1->z = z2;
    l1->dQ = dQ2;
    l1->M = filtSize*filtSize;
    l1->N = 1;

    l1->next = (Layer *) malloc(sizeof(Layer));
    l1 = l1->next;
    l1->q = l3q;
    l1->z = z3;
    l1->dQ = dQ3;
    l1->M = filtSize*filtSize;
    l1->N = 1;

    l1->next = (Layer *) malloc(sizeof(Layer));
    l1 = l1->next;
    l1->q = l4q;
    l1->z = z4;
    l1->dQ = dQ4;
    l1->M = s2;
    l1->N = s3;
    l1->next = (Layer *) malloc(sizeof(Layer));
    l1 = l1->next;
    l1->q = l5q;
    l1->z = z5;
    l1->M = s3;
    l1->N = s4;
    l1->next = NULL;

    l1 = fLayer->next;
    l1->weights = &convW1[0][0];
    l1->bias = b1;

    l1 = l1->next;
    l1->weights = &convW2[0][0];
    l1->bias = b2;

    l1 = l1->next;
    l1->weights = &weights2[0][0];
    l1->bias = b3;

    l1 = l1->next;
    l1->weights = &weights3[0][0];
    l1->bias = b4;

    initWeights(fLayer->next);

    float outVec[3];

    for(int i = 0; i < val_size; i++) {
        val[i] = data[i];
    }
    
    for(int h = 0; h < epochs; h++) {
        shuffle(data, nOfData);

        float Eloss = 0;
        for(int k = 0; k < batches-1; k++) {

            uint16_t tr_ind = 0;
            uint16_t hol_ind = 0;

            for(int i = val_size; i < nOfData; i++) {
                uint16_t uBound = val_size*2 + (val_size*k);
                uint16_t lBound = val_size + (k*val_size);
                if (i < uBound && i > lBound) {
                    hold_out[hol_ind] = data[i];
                    hol_ind++;
                } else {
                    tra[tr_ind] = data[i];
                    tr_ind++;
                }
            }

            for(int i = 0; i < training_size; i++) {
                fLayer->q = tra[i].x;
                forwardPass(fLayer, outVec, tra[i].y, lr);
            }

            float acc = 0;
            float loss = 0;
            int guess = -1;

            for(int i = 0; i < val_size; i++) {
                fLayer->q = hold_out[i].x;
                loss += validate(fLayer, outVec, hold_out[i].y);

                int guess = -1;
                guess = find_maximum(outVec, 3);
                //printf("T: %d, G: %d \n", (int)hold_out[i].y, guess);
                if(guess == (int)hold_out[i].y) {
                    acc++;
                }
            }   
        printf("Acc: %f, Loss: %f \n", acc/val_size, loss/val_size);
        if(acc/val_size > 0.8 && loss/val_size < 0.4) {
            break;
        }
        }
    }

    float acc = 0;
    float loss = 0;
    int guess = -1;

    for(int i = 0; i < val_size; i++) {
        fLayer->q = val[i].x;
        loss += validate(fLayer, outVec, val[i].y);

        int guess = -1;
        guess = find_maximum(outVec, 3);
        //printf("T: %d, G: %d \n", (int)val[i].y, guess);
        if(guess == (int)val[i].y) {
            acc++;
        }
    }   
    printf("Validation Acc: %f, Loss: %f \n", acc/val_size, loss/val_size);
    char fname[100] = "C:/Users/rytte/OneDrive/Dokument/GitHub/Algorithms/w.csv";
    writeData(fLayer->next, loss/val_size, acc/val_size, fname);

}

void train_test(struct Layer* model, float x, float y){

}

void makeModel(struct Layer* fLayer, size_t inpSize, float *w1, float *w2, float* convW1, float *convW2) {
    srand(time(NULL));

    size_t filtSize = 2;

    /* Conv layer sizes */
    size_t s1 = (int) ((inpSize - (filtSize*2))/filtSize);

    size_t s2 = (int) ((s1 - (filtSize*2))/filtSize);

    /* Dense layer sizes */
    size_t s3 = 5;
    size_t s4 = 3;

    /* Filter 2 Sizes */
    //float weights2[s2][s3];
    //float weights3[s3][s4];

    //float convW1[filtSize][filtSize];
    //float convW2[filtSize][filtSize];

    float b1[1];
    float b2[1];
    float b3[s3];
    float b4[s4];

    float x1[inpSize];
    
    float l2q[s1];
    float l3q[s2];
    float l4q[s3];
    float l5q[s4];

    float z2[s1];
    float z3[s2];
    float z4[s3];
    float z5[s4];

    float dQ4[s3];
    float dQ3[s2];
    float dQ2[s1];
    float dQ1[inpSize];
    
    //Layer *fLayer = NULL;
    fLayer = (Layer *) malloc(sizeof(Layer));
    Layer *l1 = fLayer;
    l1->q = x1;
    l1->M = inpSize;
    l1->inputSize = inpSize;
    l1->dQ = dQ1;

    l1->next = (Layer *) malloc(sizeof(Layer));
    l1 = l1->next;
    l1->q = l2q;
    l1->z = z2;
    l1->dQ = dQ2;
    l1->M = filtSize*filtSize;
    l1->N = 1;

    l1->next = (Layer *) malloc(sizeof(Layer));
    l1 = l1->next;
    l1->q = l3q;
    l1->z = z3;
    l1->dQ = dQ3;
    l1->M = filtSize*filtSize;
    l1->N = 1;

    l1->next = (Layer *) malloc(sizeof(Layer));
    l1 = l1->next;
    l1->q = l4q;
    l1->z = z4;
    l1->dQ = dQ4;
    l1->M = s2;
    l1->N = s3;
    l1->next = (Layer *) malloc(sizeof(Layer));
    l1 = l1->next;
    l1->q = l5q;
    l1->z = z5;
    l1->M = s3;
    l1->N = s4;
    l1->next = NULL;

    l1 = fLayer->next;
    l1->weights = &convW1;
    l1->bias = b1;

    l1 = l1->next;
    l1->weights = &convW2;
    l1->bias = b2;

    l1 = l1->next;
    l1->weights = &w1;
    l1->bias = b3;

    l1 = l1->next;
    l1->weights = &w2;
    l1->bias = b4;

    //return fLayer;
}

void delModel(struct Layer* head) {
    struct Layer* tmp;
    while(head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

int main() {

    int nOfTrainData = 50*3;

    struct my_data2 symb[nOfTrainData];
    struct my_data symbols[nOfTrainData];
    readData(symbols, 50);

    readData(symb, 50);

    shuffle(symbols, nOfTrainData);

    shuffle(symb, nOfTrainData);
    //train(symbols, nOfTrainData, 25, 0.002, 80);   
    size_t filtSize = 2;

    /* Conv layer sizes */
    size_t firstInpSize = 80;
    size_t s1 = (int) ((firstInpSize - (filtSize*2))/filtSize);

    size_t s2 = (int) ((s1 - (filtSize*2))/filtSize);

    /* Dense layer sizes */
    size_t s3 = 5;
    size_t s4 = 3;
    float weights2[s2][s3];
    float weights3[s3][s4];

    float convW1[filtSize][filtSize];
    float convW2[filtSize][filtSize];

    Layer *fLayer = NULL;
    makeModel(fLayer, firstInpSize,weights2, weights3, convW1, convW2);

    uint8_t i;
    for(i = 0; i < nOfTrainData; i++) {
        float *data = symbols[i].x;
        uint8_t y = symbols[i].y;
        train_test(data, y, 0.002, 80);
    } 

    return 0;
}