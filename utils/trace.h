// Library used to print some STL structures on C++

template <typename T, typename S> 
ostream& operator<<(ostream& os, const pair<T, S>& v) 
{ 
    os << "("; 
    os << v.first << ", " 
       << v.second << ")"; 
    return os; 
} 

template <typename T> 
ostream& operator<<(ostream& os, const vector<T>& v) 
{ 
    os << "["; 
    for (int i = 0; i < v.size(); ++i) { 
        os << v[i]; 
        if (i != v.size() - 1) 
            os << ", "; 
    } 
    os << "]"; 
    return os; 
}

template <typename T> 
ostream& operator<<(ostream& os, const set<T>& v) 
{ 
    os << "{"; 
    for (auto it : v) { 
        os << it; 
        if (it != *v.rbegin()) 
            os << ", "; 
    } 
    os << "}"; 
    return os; 
}

template <typename T, typename S> 
ostream& operator<<(ostream& os, const map<T, S>& v) 
{ 
    os << "{"; 
    for (auto it : v) {
        os << "(" << it.first << " : "  << it.second << ")"; 
        if (it != *v.rbegin()) 
            os << ", ";
    } 
    os << "}";
    return os; 
}


// Trace dinâmico
void trace() { cout << endl; }
template<typename T, typename... Args>
void trace(T a, Args... args) { cout << a << " "; trace(args...); }

// Debugador de recursão
int recNum = 0;
void prTabs() { for (int ids = 0; ids < recNum; ++ids) cout << "    "; }
template<typename... Args>
void recInit(Args... args) { prTabs(); trace("rec(", args..., ")"); recNum++; }
template<typename... Args>
void recEnd(Args... args) { prTabs(); trace("->", args...); recNum--;}
template<typename... Args>
void recInfo(Args... args) { prTabs(); trace(": ", args...);}