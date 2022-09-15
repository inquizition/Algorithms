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

static struct Messagwe divide(struct Message m_1, struct Message m_2)
{
    
}