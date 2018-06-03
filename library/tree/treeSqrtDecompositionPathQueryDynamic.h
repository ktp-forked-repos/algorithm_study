#pragma once

template <typename T, typename MergeOp = function<T(T, T)>>
struct DynamicTreeSqrtDecompositionPathQuery {
    int                 N;              // the number of vertex
    int                 sqrtN;          // sqrt(N)

    vector<set<int>>    edges;          // edges (vertex number)

    vector<int>         level;          // level (root is 0)

    vector<int>         parent;         // 
    vector<int>         jump;           // to a ancestor

    vector<T>           values;         // 
    vector<T>           sqrtValues;     // 
    T                   defaultValue;   // 
    MergeOp             mergeOp;        // 

    DynamicTreeSqrtDecompositionPathQuery(MergeOp op, T dflt = T())
        : N(0), sqrtN(0), mergeOp(op), defaultValue(dflt) {
    }

    explicit DynamicTreeSqrtDecompositionPathQuery(int n, MergeOp op, T dflt = T(), int sqrtN = 0)
        : mergeOp(op), defaultValue(dflt) {
        init(n, sqrtN);
    }

    void init(int _n, int _sqrtN = 0) {
        N = _n;
        sqrtN = _sqrtN;
        if (sqrtN <= 0)
            sqrtN = (int)sqrt(N);

        edges = vector<set<int>>(N);
        level.assign(N, 0);
        parent.assign(N, -1);
        jump.assign(N, -1);

        values.assign(N, defaultValue);
        sqrtValues.assign(N, defaultValue);
    }

    //--- build

    // before calling build()
    void addEdge(int u, int v) {
        edges[u].insert(v);
        edges[v].insert(u);
    }

    // before calling build()
    void setValue(int u, const T& value) {
        values[u] = value;
    }

    // O(N)
    void build(int root) {
        dfsBuild(root, -1);
    }

    //--- lca

    // O(sqrt(N))
    int lca(int u, int v) const {
        while (jump[u] != jump[v]) {
            if (level[u] > level[v])
                swap(u, v);
            v = jump[v];
        }
        return lcaNaive(u, v);
    }

    // dist >= 0, O(sqrt(N))
    int climb(int u, int dist) const {
        int p = jump[u];
        while (p >= 0 && (level[u] - level[p]) <= dist) {
            dist -= (level[u] - level[p]);
            u = p;
            p = jump[u];
        }
        return climbNaive(u, dist);
    }

    // kth >= 1, O(sqrt(N))
    // count = valueToCountF(index_of_node, value_or_sqrt_value_of_the_node)
    int climbKth(int u, int kth, const function<int(T)>& valueToCountF) const {
        int p = jump[u];
        int d;
        while (p >= 0 && (d = valueToCountF(sqrtValues[u])) < kth) {
            kth -= d;
            u = p;
            p = jump[u];
        }
        return climbKthNaive(u, kth, valueToCountF);
    }

    //--- update

    // worst case O(N)
    void update(int u, const T& val) {
        if (values[u] != val) {
            values[u] = val;
            dfsUpdate(u, parent[u]);
        }
    }

    // worst case O(N)
    void updateSubtree(int u, const T& val) {
        dfsUpdateSubtree(u, parent[u], val);
    }

    // worst case O(N)
    void add(int u, const T& val) {
        values[u] += val;
        dfsUpdate(u, parent[u]);
    }

    // worst case O(N)
    void addSubtree(int u, const T& val) {
        dfsAddSubtree(u, parent[u], val);
    }


    // worst case O(N)
    // PRECONDITION: u must be not root
    // PRECONDITION: newParent >= 0
    void changeParent(int u, int newParent) {
        if (newParent == parent[u])
            return;

        if (parent[u] >= 0) {
            edges[u].erase(parent[u]);
            edges[parent[u]].erase(u);
        }

        addEdge(u, newParent);
        parent[u] = newParent;
        dfsBuild(u, newParent);
    }

    //--- query

    // O(sqrt(H))
    T queryToRoot(int u) const {
        T res = sqrtValues[u];
        int p = jump[u];
        while (p >= 0) {
            res = mergeOp(res, sqrtValues[p]);
            p = jump[p];
        }
        return res;
    }

    // O(sqrt(H))
    T queryToAncestor(int u, int ancestor) const {
        T res = defaultValue;
        while (jump[u] != jump[ancestor]) {
            res = mergeOp(res, sqrtValues[u]);
            u = jump[u];
        }
        return mergeOp(res, queryToAncestorNaive(u, ancestor));
    }

    // dist >= 0, O(sqrt(H))
    T queryTowardRoot(int u, int dist) const {
        T res = defaultValue;
        int p = jump[u];
        while (p >= 0 && (level[u] - level[p]) <= dist) {
            dist -= (level[u] - level[p]);
            res = mergeOp(res, sqrtValues[u]);
            u = p;
            p = jump[u];
        }
        return mergeOp(res, queryTowardRootNaive(u, dist));
    }

    // O(sqrt(H))
    T query(int u, int v) const {
        if (u == v)
            return values[u];

        if (level[u] > level[v])
            swap(u, v);

        int lc = lca(u, v);
        if (lc == u)
            return queryToAncestor(v, u);

        return mergeOp(queryToAncestor(u, lc), queryTowardRoot(v, level[v] - level[lc] - 1));
    }

    //--- for accumulative operation
    // Use this functions when MergeOp is 'add' (supporting subtraction)

    // O(sqrt(H))
    T queryToAncestorAccumulative(int u, int ancestor) const {
        T res = defaultValue;
        while (jump[u] != jump[ancestor]) {
            res = mergeOp(res, sqrtValues[u]);
            u = jump[u];
        }
        return mergeOp(res, sqrtValues[u] - sqrtValues[ancestor] + values[ancestor]);
    }

    // O(sqrt(H))
    T queryAccumulative(int u, int v) const {
        if (u == v)
            return values[u];

        if (level[u] > level[v])
            swap(u, v);

        int lc = lca(u, v);
        if (lc == u)
            return queryToAncestorAccumulative(v, u);

        return mergeOp(queryToAncestorAccumulative(u, lc), queryToAncestorAccumulative(v, lc)) - values[lc];
    }

protected:
    void dfsBuild(int u, int par) {
        parent[u] = par;
        if (par >= 0)
            level[u] = level[par] + 1;

        if (level[u] % sqrtN == 0) {
            jump[u] = parent[u];
            sqrtValues[u] = values[u];
        } else {
            jump[u] = jump[par];
            sqrtValues[u] = mergeOp(sqrtValues[par], values[u]);
        }

        for (auto v : edges[u]) {
            if (v != par)
                dfsBuild(v, u);
        }
    }

    void dfsUpdate(int u, int par) {
        if (level[u] % sqrtN == 0)
            sqrtValues[u] = values[u];
        else
            sqrtValues[u] = mergeOp(sqrtValues[par], values[u]);

        for (auto v : edges[u]) {
            if (v != par)
                dfsUpdate(v, u);
        }
    }

    void dfsUpdateSubtree(int u, int par, const T& val) {
        values[u] = val;
        if (level[u] % sqrtN == 0)
            sqrtValues[u] = values[u];
        else
            sqrtValues[u] = mergeOp(sqrtValues[par], values[u]);

        for (auto v : edges[u]) {
            if (v != par)
                dfsUpdateSubtree(v, u, val);
        }
    }

    void dfsAddSubtree(int u, int par, const T& val) {
        values[u] += val;
        if (level[u] % sqrtN == 0)
            sqrtValues[u] = values[u];
        else
            sqrtValues[u] = mergeOp(sqrtValues[par], values[u]);

        for (auto v : edges[u]) {
            if (v != par)
                dfsAddSubtree(v, u, val);
        }
    }


    int lcaNaive(int u, int v) const {
        if (u == v)
            return u;

        if (level[u] > level[v])
            swap(u, v);

        while (level[u] != level[v])
            v = parent[v];

        while (u != v) {
            u = parent[u];
            v = parent[v];
        }
        return u;
    }

    int climbNaive(int u, int dist) const {
        while (u >= 0 && dist-- > 0)
            u = parent[u];
        return u;
    }

    int climbKthNaive(int u, int kth, const function<int(T)>& valueToCountF) const {
        while (u >= 0 && kth > 0) {
            if (valueToCountF(values[u])) {
                if (--kth == 0)
                    break;
            }
            u = parent[u];
        }
        return u;
    }

    T queryToAncestorNaive(int u, int ancestor) const {
        T res = values[u];
        if (u != ancestor) {
            do {
                u = parent[u];
                res = mergeOp(res, values[u]);
            } while (u != ancestor);
        }
        return res;
    }

    T queryTowardRootNaive(int u, int dist) const {
        T res = values[u];
        if (dist > 0) {
            do {
                u = parent[u];
                res = mergeOp(res, values[u]);
            } while (u >= 0 && --dist > 0);
        }
        return res;
    }
};

template <typename T, typename MergeOp>
inline DynamicTreeSqrtDecompositionPathQuery<T, MergeOp> makeDynamicTreeSqrtDecompositionPathQuery(MergeOp op, T dfltValue) {
    return DynamicTreeSqrtDecompositionPathQuery<T, MergeOp>(op, dfltValue);
}

template <typename T, typename MergeOp>
inline DynamicTreeSqrtDecompositionPathQuery<T, MergeOp> makeDynamicTreeSqrtDecompositionPathQuery(int size, MergeOp op, T dfltValue, int sqrtN = 0) {
    return DynamicTreeSqrtDecompositionPathQuery<T, MergeOp>(size, op, dfltValue, sqrtN);
}
