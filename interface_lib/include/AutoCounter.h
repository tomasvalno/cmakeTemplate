#ifndef   COMMON_AUTO_COUNTER_H
#define	  COMMON_AUTO_COUNTER_H

namespace common
{
    template<class T>
    class AutoCounter
    {
        public:
            AutoCounter(T & var) //(T const & val)
                : m_counter(var)
            {
                m_counter++;
            }

            ~AutoCounter()
            {
                m_counter--;
            }

        private:
            T & m_counter;
    };
}

#endif // COMMON_AUTO_COUNTER_H
