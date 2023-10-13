import numpy as np

class Trueskill:

    def __init__(self, p1, p2, prior):
        self.p1 = p1
        self.p2 = p2
        self.prior = prior
        

    def posterior(self, x, t, x_test, var):
        
        return m_N, S_N



class Message:

    def __init__(self, mu, sigma):
        self.mu = mu
        self.sigma = sigma
    
    def new_mu(self, mu):
        self.mu = mu

    def new_sigma(self, sigma):
        self.sigma = sigma
