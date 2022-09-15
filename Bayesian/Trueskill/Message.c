#include "stdint.h"
#include "Team.h"
#include "Message.h"



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

static struct Message truncMM(struct Message m, uint32_t y_obs)
{
    if(y_obs == 1)
    {

    }
    else if(y_obs == 0)
    {

    }
    else
    {

    }
    struct Message m_out;

}

static uint32_t MM_mean(uint8_t a, uint8_t alpha, uint32_t mu, uint32_t Z) 
{

    return erfc(3);
}

static uint32_t MM_var()
{

}
