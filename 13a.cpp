#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <queue>
#include <set>
#include <algorithm>


using namespace std;

class Prefix {

private:
    string s;
    vector<int> p;

public:
    //оч ориг
    void sol3() {
        //ifstream cin("prefix.in");
        cin >> s;
        this->s = s;
        vector<int>ans = func(s);
        //ofstream cout("prefix.out");
        for (auto e : ans) {
            cout << e << ' ';
        }
    }

    vector<int> func(string s) {
        this->s = s;
        p.resize(s.size());
        p[0] = 0;
        for (int i = 1; i < s.size(); ++i) {
            int j = p[i - 1];
            while (j > 0 && s[i] != s[j])
                j = p[j - 1];
            if (s[i] == s[j])  ++j;
            p[i] = j;
        }
        return p;
    }
};

class KMP:public Prefix {

private:
    string p, t, s;
    vector<int> pf;
    vector<int> answer;

public:
    
    void solve() {
        //ofstream cout("search2.out");
        fill();
        s = p + '#' +  t;
        
        pf = func(s);
        int c = 0;
        cout << "\n";
        for (int i = 0; i < pf.size(); i++) {
            cout << pf[i] << ' ';
        }
        cout << '\n';
        for (int i = p.size(); i < pf.size(); i++) {
            if (pf[i] == p.size())
                answer.push_back((int)(i + 1 - (2*p.size())));
        }
        
        cout << answer.size() << endl;
        for (auto e : answer) {
            cout << e  << ' ';
        }
    }
    void fill() {
        //ifstream cin("search2.in");
        cin >> p >> t;
    }
};

class AutoKMP:public KMP {

private:
   
    int da_power;
    string s;
    vector<int> p;
    vector<vector<int>> declaration;

public:
    
    void sol4() {
        
        fill();
        s = s + '#';
        p = func(s);
        
        for (int i = 0; i < s.size(); i++) {
            for (char ch = 'a'; ch < 'a' + da_power; ch++)
            {
                if (i > 0 && ch != s[i])
                {
                    declaration[i][ch - 'a'] = declaration[p[i - 1]][ch - 'a'];
                }
                else
                {
                    declaration[i][ch - 'a'] = i + (ch == s[i]);
                }
            }
        }

        for (auto e1 : declaration)
        {
            //aboba
            for (auto e2 : e1) {
                cout << e2 << " ";
            }
            cout << endl;
        }

    }
    
    void fill() {
        cin >> da_power >> s;
        declaration.resize(s.size() + 1, vector<int>(da_power));
    }

};

class MultSearch:public Prefix {

private:
   
    int da_power;
    vector<pair<string, bool>> ib;
    string t;
    vector<int> p;

public:
    
    void sol5() {
        fill();
        ofstream cout("search4.out");
        
        for (int i = 0; i < da_power; i++) {
             
            string s = ib[i].first + '#' + t;
            p = func(s);
            for (int j = ib[i].first.size(); j < p.size(); j++) {
                if (p[j] == ib[i].first.size()) {
                    ib[i].second = true;
                    break;
                }
            }
        }

        for (int i = 0; i < da_power; i++) {
            if (ib[i].second) {
                cout << "YES" << endl;
            }
            else {
                cout << "NO" << endl;
            }
        }

    }
    
    void fill() {
        ifstream cin("search4.in");
        cin >> da_power;
        string casual;

        for (int i = 0; i < da_power; i++) {
            cin >> casual;
            ib.push_back(make_pair(casual, false));
        }
        cin >> t;
    }

};


/// <summary>
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// </summary>


class Node {

    struct node {
        node* letters[26];
        vector<int> has_ans;
        node* suf_mail, * pred;
        int last_symb;
        node() {
            fill(letters, letters + 26, nullptr);
            suf_mail = nullptr;
            pred = nullptr;
            has_ans.clear();
            last_symb = 1;
        }
    };

    set<node*> used;
    queue<node*> q;
    vector<bool> ans;

public:

    //где мы сейчас, индекс начала, индекс слова, которое мы добавляем
    void make_bor(node* now, string& s, int ind, int j) {
        if (ind == (int)s.size()) {
            now->has_ans.push_back(j);
            return;
        }
        if (now->letters[s[ind] - 'a'] == nullptr) {
            node* new_node = new node();
            new_node->last_symb = s[ind] - 'a';
            new_node->pred = now;
            now->letters[s[ind] - 'a'] = new_node;
        }
        make_bor(now->letters[s[ind] - 'a'], s, ind + 1, j);
    }

    void go(node* now, string& s, int ind) {
        used.insert(now);
        if (ind == (int)s.size()) {
            return;
        }
        go(now->letters[s[ind] - 'a'], s, ind + 1);
    }

    void bfs(node* root) {
        queue<node*> qbfs;
        qbfs.push(root);
        node* next = qbfs.front();
        qbfs.pop();
        int next_char = next->last_symb;
        if (next == root) {
            for (int i = 0; i < 26; i++) {
                if (next->letters[i] != nullptr) {
                    qbfs.push(next->letters[i]);
                }
                else {
                    next->letters[i] = root;
                }
            }
        }
        node* now = next->pred;
        if (now == root) {
            next->suf_mail = now;
            for (int i = 0; i < 26; i++) {
                if (next->letters[i] != nullptr) {
                    qbfs.push(next->letters[i]);
                }
                else {
                    next->letters[i] = next->suf_mail->letters[i];
                }
            }
        }
        //int next_char = next->last_symb;
        next->suf_mail = next->pred->letters[next_char];
        for (int i = 0; i < 26; i++) {
            if (next->letters[i] != nullptr) {
                qbfs.push(next->letters[i]);
            }
            else {
                next->letters[i] = next->suf_mail->letters[i];
            }
        }
    }


    void start() {

        //ifstream cin("search4.in");
        //ofstream cout("search4.out");

        node* root = new node();
        // ссылочка из себя в себя на всякий
        root->pred = root;
        string s;
        int n;

        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> s;
            make_bor(root, s, 0, i + 1);
        }
        root->suf_mail = root;
        bfs(root);
        ans.resize(n);
        string maino_De_busto;
        cin >> maino_De_busto;
        go(root, s, 0);
        // $o$
        for (auto& e : used) {
            q.push(e);
        }
        node* now;
        q.pop();
        while (!q.empty()) {
            now = q.front();
            q.pop();
            // mistake 
            int k = now->has_ans.size();
            if (k != 0) {
                for (unsigned int i = 0; i < now->has_ans.size(); i++) {
                    ans[now->has_ans[i] - 1] = true;
                }
            }
            if (used.count(now->suf_mail) == 0) {
                used.insert(now->suf_mail);
                q.push(now->suf_mail);
            }
        }
        for (int j = 0; j < n; j++) {
            if (ans[j]) {
                cout << "YES" << endl;
            }
            else {
                cout << "NO" << endl;
            }
        }


    }
};

    int main()
    {
        KMP s;
        s.solve();
        //Prefix s;
        //s.sol3();
        //AutoKMP a;
        //a.sol4();
        //MultSearch ms;
        //ms.sol5();
        //Node js;
        //js.start();
    }

