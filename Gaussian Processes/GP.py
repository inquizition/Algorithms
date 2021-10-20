import numpy as np

class GP:

    def __init__(self, sigma, l):
        self.sigma = sigma
        self.l = l
        self.kern = 'gp'

    def kernel(self, x, mu):
        sqdist = np.sum(x**2, 1).reshape(-1,1) + np.sum(mu**2, 1) - 2 * np.dot(x, mu.T)    
        return self.sigma**2 * np.exp(-0.5 / self.l**2 * sqdist)

    def K(self, x1, x2):
        return self.kernel(x1, x2)

    def posterior(self, x, t, x_test, var):
        Sn_inv = (var)*np.eye(len(x)) + self.K(x,x)
        m_N = (self.K(x, x_test).T).dot(np.linalg.pinv(Sn_inv))
        S_N = (self.K(x_test,x_test) + var*np.eye(len(x_test)) - (m_N.dot(self.K(x,x_test))))
        m_N = m_N.dot(t)

        return m_N, S_N