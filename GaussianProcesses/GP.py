import numpy as np

def K(gp, x1, x2):
        return kernel(x1, x2, gp.l, gp.sigma)

def kernel(x, mu, l, sigma):
        sqdist = np.sum(x**2, 1).reshape(-1,1) + np.sum(mu**2, 1) - 2 * np.dot(x, mu.T)    
        return sigma**2 * np.exp(-0.5 / l**2 * sqdist)

class GP:

    def __init__(self, sigma, l):
        self.sigma = sigma
        self.l = l
        self.kern = 'gp'

    def posterior(self, x, t, x_test, var):
        Sn_inv = (var)*np.eye(len(x)) + K(self, x,x)
        m_N = (K(self, x, x_test).T).dot(np.linalg.pinv(Sn_inv))
        S_N = (K(self, x_test,x_test) + var*np.eye(len(x_test)) - (m_N.dot(K(self, x,x_test))))
        m_N = m_N.dot(t)

        return m_N, S_N