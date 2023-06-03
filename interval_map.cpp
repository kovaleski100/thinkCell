#include <map>
#include <iostream>
template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
        if(!(keyBegin < keyEnd))
        {
            return ;
        }

        auto it = m_map.upper_bound(keyBegin);

        if(it == m_map.begin() && (keyBegin < it->first || it == m_map.end()))
        {   
            if(m_valBegin == val)
            {
                return;
            }
            m_map.insert(it, {keyBegin, val});
        }
        else if(std::prev(it)->second == val)
        {
            return;
        }
        else if(it != m_map.end() && keyEnd < it->first && it->second == val)
        {
            return;
        }
        else
        {
            V const& prevVal = std::prev(it)->second;
            if(prevVal == val)
            {
                --it;
                K newKeyEnd = std::max(keyEnd, it->first);
                it = m_map.erase(it);
                if(newKeyEnd > keyBegin)
                {
                    m_map.insert(it, {newKeyEnd, prevVal});
                }
            }
            while(it != m_map.end() && it->second == val && keyEnd > it->first)
            {
                it = m_map.erase(it);
            }
            if(it != m_map.end() && keyEnd > it->first && it->second == val)
            {
                K newKeyEnd = std::max(keyEnd, it->first);
                it = m_map.erase(it);
                if(newKeyEnd > keyBegin)
                {
                    m_map.insert(it, {newKeyEnd, prevVal});
                }
            }
            m_map.insert(it, {keyBegin, val});
            if(keyEnd > keyBegin)
            {
                m_map.insert(it, {keyEnd, prevVal});
            }
        }

    }

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

int main() {
    interval_map<int, char> M('A');

    //M.assign(1, 3, 'B');
    M.assign(5, 7, 'C');
    //M.assign(2, 6, 'D');

    for (int i = -2; i <= 8; i++) {
        std::cout << i << " -> " << M[i] << std::endl;
    }

    return 0;
}
// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.