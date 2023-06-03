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
// INSERT YOUR SOLUTION HERE
		if (!(keyBegin < keyEnd)) {
			return;
		}

		auto it = m_map.upper_bound(keyBegin);

		// Handle special case if keyBegin is smaller than any key in the map
		if (it == m_map.begin() && (keyBegin < it->first || it == m_map.end())) {
			if (m_valBegin == val)
				return;
			m_map.insert(it, { keyBegin, val });
		}

		// Check if keyBegin falls within the previous interval
		else if (std::prev(it)->second == val)
			return;

		// Check if keyEnd falls within the current interval
		else if (it != m_map.end() && keyEnd < it->first && it->second == val)
			return;

		// Insert or update the intervals as necessary
		else {
			V const& prevVal = std::prev(it)->second;
			if (prevVal == val) {
				// Merge with the previous interval if it has the same value
				--it;
				keyEnd = std::max(keyEnd, it->first);
				it = m_map.erase(it);
			}

			while (it != m_map.end() && it->second == val && keyEnd > it->first) {
				// Erase intervals completely covered by the new interval
				it = m_map.erase(it);
			}

			if (keyEnd > it->first && it->second == val) {
				// Merge with the following interval if it has the same value
				keyEnd = std::max(keyEnd, it->first);
				it = m_map.erase(it);
			}

			m_map.insert(it, { keyBegin, val });  // Insert the new interval
			if (keyEnd > keyBegin)
				m_map.insert(it, { keyEnd, prevVal });  // Insert the end of the previous interval
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

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

int main() {
    interval_map<int, char> M('A');  // Create an interval map with initial value 'A'

    M.assign(1, 3, 'B');  // Assign value 'B' to the interval [1, 3)
    M.assign(5, 7, 'C');  // Assign value 'C' to the interval [5, 7)
    M.assign(2, 6, 'D');  // Assign value 'D' to the interval [2, 6)

    // Print the mappings
    for (int i = -2; i <= 8; i++) {
        std::cout << i << " -> " << M[i] << std::endl;
    }

    return 0;
}
