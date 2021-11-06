import numpy as np
import matplotlib.pyplot as plt

def LMS(input, y, p, mu):

    h = np.zeros([p,1])
    x = np.flip(input[0:p])
    e = np.zeros(len(input)-p)

    for i in range(int(len(input) - 2*p+2)):
        e[i] = y[i] - (h.T).dot(x)
        h = h + (mu*e[i]*x)

        x = np.flip(input[p+i:2*p + i])
    return e

def NLMS(input, y, p, mu):
    
    h = np.zeros([p,1])
    x = np.flip(input[0:p])
    e = np.zeros(len(input)-p)

    for i in range(int(len(input) - 2*p+2)):
        e[i] = y[i] - (h.T).dot(x)
        h = h + (mu*e[i]*x)/((x.T).dot(x))

        x = np.flip(input[p+i:2*p + i])
    return e

def RLS(input, y, p, lda, ro):
    
    w = np.zeros([p,1])
    P = np.eye(p)*ro
    e = np.zeros(len(input)-p)
    x = np.flip(input[0:p])

    R = np.linalg.pinv(lda + (x[0:p].T).dot(P).dot(x))
    g = P.dot(x).dot(R)
    P = (P - g.dot(x.T).dot(P))*(1/lda)
    w = y[0]*g

    for i in range(len(input)- 2*p+2):
        #i = i +1
        e[i] = y[i] - (x.T).dot(w)
        R = np.linalg.pinv(lda + (x.T).dot(P).dot(x))
        g = P.dot(x).dot(R)
        P = (P - g.dot(x.T).dot(P))*(1/lda)
        w = w + e[i]*g
        x = np.flip(input[p+i:2*p + i])
    return e