#include "stdint.h"
#include "Team.h"
#include "Message.h"


void update_skills_MP(struct Message t1,struct Message t2, float var_t, float y_obs)
{
    struct Message mu1 = { .mu = t1.mu, .var = t1.var};
    struct Message mu1 = { .mu = t2.mu, .var = t2.var};
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

static struct Message truncMM(struct Message m, float y_obs)
{
    struct Message m_out;
    if(y_obs == 1)
    {
        float a = 0.0;
        float b = 100;
        m_out.mu = MM_mean(a,b,m.mu,m.var);
        m_out.var = MM_var(a,b,m.mu,m.var);
    }
    else if(y_obs == 0)
    {
        float a = 100;
        float b = 0.0;
        m_out.mu = MM_mean(a,b,m.mu,m.var);
        m_out.var = MM_var(a,b,m.mu,m.var);
    }
    else
    {
        print("error!");
    }
    return m_out;

}

static float stdNormalDist(Z)
{
    return exp(((pow(-Z,2))/2))/(sqrt(2*3.1415));
}

static float MM_mean(float a, float b, float mu, float var) 
{
    if(a == 0.0)
    {
        float alpha = (a-mu)/var;
        float Z = 1 - ((1/2)*(2 - erfc(alpha/(sqrt(2)))));
        return mu + var*stdNormalDist(alpha)/Z;
    }
    else if(b == 0.0)
    {
        float beta = (b - mu)/var;
        float Z = ((1/2)*(2 - erfc(beta/(sqrt(2)))));
        return mu - var*(stdNormalDist(beta)/Z);
    }
    else
    {
        print("neither 0.0");
        return -1;
    }
    
}

static float MM_var(float a, float b, float mu, float var)
{
    if(a == 0)
    {
        float alpha = (a - mu)/var;
        float Z = 1 - ((1/2)*(2 - erfc(alpha/(sqrt(2)))));
        return pow(var,2) * (1 + ((alpha*(stdNormalDist(alpha))/Z)) - pow((stdNormalDist(alpha)/Z),2));
    }
    else if(b == 0)
    {
        float beta = (b - mu)/var;
        float Z = ((1/2)*(2 - erfc(beta/(sqrt(2)))));
        return pow(var,2) * (1 - (beta*(stdNormalDist(beta)/Z)) - pow((stdNormalDist(beta)/Z),2));
    }
    else
    {
        return -1;
    }
}
