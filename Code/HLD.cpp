//я пажылой hld
struct HeavyLightDecomposition {
    struct SegTree {
        vector<int> tree;
        int n = 1;
        void init(int k){
            n = 1;
            while(n < k)n *= 2;
            tree.resize(2 * n - 1, -1);
        }
        void build(vector<int> &a, int x, int l, int r){
            if(r - l == 1){
                if(l < sz(a))tree[x] = a[l];
                return;
            }
            int m = (r + l) / 2;
            build(a, 2 * x + 1, l, m);
            build(a, 2 * x + 2, m, r);
            tree[x] = max(tree[2 * x + 1], tree[2 * x + 2]);
        }
        void upd(int x, int i, int v, int l, int r){
            if(r - l == 1){
                tree[x] = v;
                return;
            }
            int m = (r + l) / 2;
            if(i < m)upd(2 * x + 1,  i, v, l, m);
            else upd(2 * x + 2, i, v, m, r);
            tree[x] = max(tree[2 * x + 1], tree[2 * x + 2]);
        }
        int get(int x, int lx, int rx, int l, int r){
            if(lx >= r || l >= rx)return 0;
            if(l >= lx && r <= rx)return tree[x];
            int m = (r + l) / 2;
            return max(get(2 * x + 1, lx, rx, l, m), get(2 * x + 2, lx, rx, m, r));
        }
    };
    vector<int> tin;
    vector<int> tout;
    vector<vector<int>> g;
    vector<int> sz;
    vector<int> p;
    vector<int> a;
    vector<int> head;
    SegTree t;
    int n = 1;
    int timer = 0;
    void sizes(int u, int pr = -1){
        sz[u] = 1;
        p[u] = pr;
        int ind = 0;
        for(int i = 0;i < sz(g[u]);i++){
            auto v = g[u][i];
            if(v == pr)continue;
            sizes(v, u);
            sz[u] += sz[v];
            if(sz[v] > sz[g[u][ind]]){
                ind = i;
            }
        }
        if(sz(g[u]) > 0)swap(g[u][ind], g[u][0]);
    }
    void update(int v, int value){
        t.upd(0, tin[v], value, 0, t.n);
    }
    bool is_ancestor(int u, int v){
        //if u - is ancestor  of v
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }
    void hld(int u, int p = -1){
        tin[u] = timer++;
        for(auto v: g[u]){
            if(v == p)continue;
            head[v] = (v == g[u][0] ? head[u] : v);
            hld(v, u);
        }
        tout[u] = timer;
    }
    void init(int k){
        n = k;
        head.resize(n);
        tin.resize(n);
        tout.resize(n);
        sz.resize(n);
        p.resize(n, -1);
        t.init(n);
    }
    void build(vector<vector<int>> &gx, vector<int> &ax){
        g = gx;
        a = ax;
        init(sz(ax));
        sizes(0);
        hld(0);
        for(int i = 0;i < n;i++){
            update(i, a[i]);
        }
    }
    void up(int &u, int &v, int &ans){
        while(!is_ancestor(head[u], v)){
            ans = max(ans, t.get(0, tin[head[u]], tin[u] + 1, 0, t.n));
            u = p[head[u]];
        }
    }
    int query(int u, int v){
        int ans = 0;
        up(u, v, ans);
        up(v, u, ans);
        if(!is_ancestor(u, v)){
            swap(u, v);
        }
        ans = max(ans, t.get(0, tin[u], tin[v] + 1, 0, t.n));
        return ans;
    }
};