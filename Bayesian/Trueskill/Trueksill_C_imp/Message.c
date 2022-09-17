#include "stdint.h"
#include "Message.h"
#include <math.h>
#include <stdio.h>

static struct Message multiply(struct Message m_1, struct Message m_2);
static struct Message divide(struct Message m_1, struct Message m_2);
static struct Message truncMM(struct Message m, float y_obs);
static float stdNormalDist(float Z);
static float MM_mean(float a, float b, float mu, float var);
static float MM_var(float a, float b, float mu, float var);
static void printMessage(struct Message m);



float update_skills_MP(struct Message *t1,struct Message *t2, float var_t, float y_obs)
{
    struct Message mu1 = { .mu = t1->mu, .var = t1->var};
    struct Message mu2 = { .mu = t2->mu, .var = t2->var};
    //printMessage(mu2);
    struct Message mu3 = mu1;
    struct Message mu4 = mu2; 
    struct Message mu5 = { .mu = (t1->mu - t2->mu), .var = (t1->var + t2->var + var_t)}; 
    //printMessage(mu5);
    struct Message marginal_t = truncMM(mu5, y_obs); 
    //printMessage(marginal_t);
    struct Message mu9 = divide(marginal_t, mu5);
    struct Message mu10 = { .mu = mu9.mu + mu2.mu, .var = var_t + mu2.var + mu9.var};
    struct Message mu11 = { .mu = mu1.mu - mu9.mu, .var = var_t + mu1.var + mu9.var};
    struct Message marginal_s1 = multiply(mu10, mu1);
    struct Message marginal_s2 = multiply(mu11, mu2);
    t1->mu = marginal_s1.mu;
    t1->var = marginal_s1.var;
    t2->mu = marginal_s2.mu;
    t2->var = marginal_s2.var;
    return mu5.var;
}
static struct Message multiply(struct Message m_1, struct Message m_2)
{
    struct Message m_out;
    m_out.mu = (m_1.mu*m_2.var + m_2.mu*m_1.var) / (m_2.var+m_1.var);
    m_out.var = (m_2.var*m_1.var) / (m_2.var + m_1.var);
    return m_out;
}

static struct Message divide(struct Message m_1, struct Message m_2)
{
    struct Message m_out;
    m_out.mu = (m_1.mu*m_2.var - m_2.mu*m_1.var) / (m_2.var - m_1.var);
    m_out.var = (m_2.var*m_1.var) / (m_2.var - m_1.var);
    return m_out;
}

static void printMessage(struct Message m)
{
    printf("Mean: %.6f, Var: %.6f\n", m.mu, m.var);
}

static struct Message truncMM(struct Message m, float y_obs)
{
    struct Message m_out;
    if(y_obs == 1)
    {
        float a = 0.0;
        float b = -2.0/0.0;
        m_out.mu = MM_mean(a,b,m.mu,m.var);
        m_out.var = MM_var(a,b,m.mu,m.var);
    }
    else if(y_obs == 0)
    {
        float a = 2.0/0.0;
        float b = 0.0;
        m_out.mu = MM_mean(a,b,m.mu,m.var);
        m_out.var = MM_var(a,b,m.mu,m.var);
    }
    else
    {
        printf("error!");
    }
    return m_out;

}

static float stdNormalDist(float Z)
{
    return exp(((pow(-Z,2))/2))/(sqrt(2*3.1415));
}

static float MM_mean(float a, float b, float mu, float var) 
{
    if(a == 0.0)
    {
        float alpha = (a-mu)/var;
        float Z = 1.0 - ((1.0/2.0)*(2.0 - erfc(alpha/(sqrt(2.0)))));
        return mu + var*stdNormalDist(alpha)/Z;
    }
    else if(b == 0.0)
    {
        float beta = (b - mu)/var;
        float Z = ((1.0/2.0)*(2.0 - erfc(beta/(sqrt(2.0)))));
        return mu - var*(stdNormalDist(beta)/Z);
    }
    else
    {
        printf("neither 0.0");
        return -1.0;
    }
    
}

static float MM_var(float a, float b, float mu, float var)
{
    if(a == 0.0)
    {
        float alpha = (a - mu)/var;
        float Z = 1.0 - ((1.0/2.0)*(2.0 - erfc(alpha/(sqrt(2.0)))));
        float phi_z = stdNormalDist(alpha)/Z;
        return pow(var,2) * (1.0 + (alpha*phi_z) - pow((phi_z),2));
    }
    else if(b == 0.0)
    {
        float beta = (b - mu)/var;
        float Z = ((1.0/2.0)*(2.0 - erfc(beta/(sqrt(2.0)))));
        float phi_phi = stdNormalDist(beta)/Z;
        return pow(var,2) * (1.0 - (beta*(phi_phi)) - pow((phi_phi),2));
    }
    else
    {
        return -1.0;
    }
}
