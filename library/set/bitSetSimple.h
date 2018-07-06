#pragma once

// BitSet class without mBitCnt member variable
struct BitSetSimple {
    static const int BIT_SIZE = sizeof(unsigned) * 8;
    static const unsigned BIT_ALL = (unsigned)-1;
    static const unsigned BIT_ONE = 1u;

    static const int INDEX_MASK = 0x1F;
    static const int INDEX_SHIFT = 5;

    int N;

    unsigned endMask;
    vector<unsigned> values;

    BitSetSimple() {
    }

    explicit BitSetSimple(int n) {
        init(n);
    }

    void init(int n) {
        N = n;

        int r = N % BIT_SIZE;
        endMask = r ? ((BIT_ONE << r) - BIT_ONE) : BIT_ALL;
        values = vector<unsigned>((N + BIT_SIZE - 1) / BIT_SIZE);
    }

    int size() const {
        return N;
    }

    int count() const {
        int res = 0;
        for (int i = 0; i < (int)values.size(); i++)
            res += popCount(values[i]);
        return res;
    }

    bool all() const {
        return count() >= N;
    }

    bool any() const {
        return count() > 0;
    }

    bool none() const {
        return count() <= 0;
    }

    bool test(int pos) const {
        return (values[pos >> INDEX_SHIFT] & (BIT_ONE << (pos & INDEX_MASK))) != 0;
    }

    BitSetSimple& set() {
        int n = N >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            values[i] = BIT_ALL;
        if (endMask + 1 != 0)
            values[n] = endMask;
        return *this;
    }

    BitSetSimple& set(int pos, bool value = true) {
        int idx = pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;
        if (value) {
            if ((values[idx] & (BIT_ONE << off)) == 0)
                values[idx] |= BIT_ONE << off;
        } else {
            if (values[idx] & (BIT_ONE << off))
                values[idx] &= ~(BIT_ONE << off);
        }
        return *this;
    }

    BitSetSimple& reset() {
        int n = (N + BIT_SIZE - 1) >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            values[i] = 0;
        return *this;
    }

    BitSetSimple& reset(int pos) {
        int idx = pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;
        if (values[idx] & (BIT_ONE << off))
            values[idx] &= ~(BIT_ONE << off);

        return *this;
    }

    BitSetSimple& flip() {
        int n = N >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            values[i] ^= BIT_ALL;
        if (endMask + 1 != 0)
            values[n] ^= endMask;
        return *this;
    }

    BitSetSimple& flip(int pos) {
        int idx = pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;
        if ((values[idx] & (BIT_ONE << off)) == 0)
            values[idx] |= BIT_ONE << off;
        else
            values[idx] &= ~(BIT_ONE << off);
        return *this;
    }

    bool operator ==(const BitSetSimple& rhs) const {
        int n1 = (N + BIT_SIZE - 1) >> INDEX_SHIFT;
        int n2 = (rhs.N + BIT_SIZE - 1) >> INDEX_SHIFT;
        if (n1 != n2)
            return false;

        for (int i = 0; i < n1; i++) {
            if (values[i] != rhs.values[i])
                return false;
        }
        return true;
    }

    bool operator !=(const BitSetSimple& rhs) const {
        return !operator ==(rhs);
    }

    BitSetSimple& operator |=(const BitSetSimple& rhs) {
        int n1 = (N + BIT_SIZE - 1) >> INDEX_SHIFT;
        int n2 = (rhs.N + BIT_SIZE - 1) >> INDEX_SHIFT;

        int n = min(n1, n2);
        for (int i = 0; i < n; i++)
            values[i] |= rhs.values[i];

        return *this;
    }

    BitSetSimple& operator &=(const BitSetSimple& rhs) {
        int n1 = (N + BIT_SIZE - 1) >> INDEX_SHIFT;
        int n2 = (rhs.N + BIT_SIZE - 1) >> INDEX_SHIFT;

        int n = min(n1, n2);
        for (int i = 0; i < n; i++)
            values[i] &= rhs.values[i];

        return *this;
    }

    BitSetSimple& operator ^=(const BitSetSimple& rhs) {
        int n1 = (N + BIT_SIZE - 1) >> INDEX_SHIFT;
        int n2 = (rhs.N + BIT_SIZE - 1) >> INDEX_SHIFT;

        int n = min(n1, n2);
        for (int i = 0; i < n; i++)
            values[i] ^= rhs.values[i];

        return *this;
    }

    BitSetSimple operator |(const BitSetSimple& rhs) {
        BitSetSimple bs = *this;
        bs.init(N);

        int n = N >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            bs.values[i] = values[i] | rhs.values[i];
        if (endMask + 1 != 0)
            bs.values[n] = (values[n] | rhs.values[n]);

        return bs;
    }

    BitSetSimple operator &(const BitSetSimple& rhs) {
        BitSetSimple bs = *this;
        bs.init(N);

        int n = N >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            bs.values[i] = values[i] & rhs.values[i];
        if (endMask + 1 != 0)
            bs.values[n] = values[n] & rhs.values[n];

        return bs;
    }

    BitSetSimple operator ^(const BitSetSimple& rhs) {
        BitSetSimple bs = *this;
        bs.init(N);

        int n = N >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            bs.values[i] = values[i] ^ rhs.values[i];
        if (endMask + 1 != 0)
            bs.values[n] = values[n] ^ rhs.values[n];

        return bs;
    }

    BitSetSimple operator ~() const {
        BitSetSimple bs = *this;
        bs.init(N);

        int n = N >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            bs.values[i] = ~values[i];
        if (endMask + 1 != 0)
            bs.values[n] = ~values[n] & endMask;

        return bs;
    }

    BitSetSimple& operator <<=(int n) {
        if (n <= 0)
            return *this;

        if (n >= N) {
            fill(values.begin(), values.end(), 0);
            return *this;
        }

        int d = n >> INDEX_SHIFT;
        int r = n & INDEX_MASK;

        if (r == 0) {
            int t = (N - 1) >> INDEX_SHIFT;
            int s = t - d;

            while (s >= 0)
                values[t--] = values[s--];
            while (t >= 0)
                values[t--] = 0;
        } else {
            int t = (N - 1) >> INDEX_SHIFT;
            int s1 = t - d;
            int s0 = s1 - 1;

            while (s0 >= 0)
                values[t--] = (values[s1--] << r) | (values[s0--] >> (BIT_SIZE - r));
            if (s1 >= 0)
                values[t--] = (values[s1--] << r);
            while (t >= 0)
                values[t--] = 0;
        }
        if (endMask + 1 != 0)
            values[N >> INDEX_SHIFT] &= endMask;

        return *this;
    }

    BitSetSimple& operator >>=(int n) {
        if (n <= 0)
            return *this;

        if (n >= N) {
            fill(values.begin(), values.end(), 0);
            return *this;
        }

        int d = n >> INDEX_SHIFT;
        int r = n & INDEX_MASK;

        int VN = (int)values.size();

        if (r == 0) {
            int t = 0;
            int s = t + d;

            while (s < VN)
                values[t++] = values[s++];
            while (t < VN)
                values[t++] = 0;
        } else {
            int t = 0;
            int s0 = t + d;
            int s1 = s0 + 1;

            while (s1 < VN)
                values[t++] = (values[s0++] >> r) | (values[s1++] << (BIT_SIZE - r));
            if (s0 < VN)
                values[t++] = (values[s0++] >> r);
            while (t < VN)
                values[t++] = 0;
        }
        if (endMask + 1 != 0)
            values[N >> INDEX_SHIFT] &= endMask;

        return *this;
    }

    //-----------------------------------------------------

    int firstClearBit() const {
        for (int i = 0; i < (int)values.size(); i++) {
            if (values[i] != BIT_ALL) {
                int m = (int)~values[i];
                return i * BIT_SIZE + BIT_SIZE - clz(unsigned(m & -m)) - 1;
            }
        }
        return size();
    }

    int first() const {
        for (int i = 0; i < (int)values.size(); i++) {
            if (values[i]) {
                int m = (int)values[i];
                return i * BIT_SIZE + BIT_SIZE - clz(unsigned(m & -m)) - 1;
            }
        }
        return -1;
    }

    int last() const {
        for (int i = (int)values.size() - 1; i >= 0; i--) {
            if (values[i])
                return i * BIT_SIZE + BIT_SIZE - clz(values[i]) - 1;
        }
        return -1;
    }

    // pos < next(pos) < N (or -1)
    int next(int pos) const {
        if (++pos >= N)
            return -1;

        int index = pos >> INDEX_SHIFT;
        int offset = pos & INDEX_MASK;

        int m = (int)values[index] & (BIT_ALL << offset);
        if (m)
            return (index << INDEX_SHIFT) + BIT_SIZE - clz(unsigned(m & -m)) - 1;

        for (int i = index + 1; i < (int)values.size(); i++) {
            if (values[i]) {
                m = (int)values[i];
                return (i << INDEX_SHIFT) + BIT_SIZE - clz(unsigned(m & -m)) - 1;
            }
        }

        return -1;
    }

    // -1 <= prev(pos) < pos
    int prev(int pos) const {
        if (--pos < 0)
            return -1;

        int index = pos >> INDEX_SHIFT;
        int offset = pos & INDEX_MASK;

        int m = (int)values[index] & (BIT_ALL >> (BIT_SIZE - 1 - offset));
        if (m)
            return (index << INDEX_SHIFT) + BIT_SIZE - clz(m) - 1;

        for (int i = index - 1; i >= 0; i--) {
            if (values[i])
                return (i << INDEX_SHIFT) + BIT_SIZE - clz(values[i]) - 1;
        }

        return -1;
    }

    static int clz(unsigned x) {
#ifndef __GNUC__
        return (int)__lzcnt(x);
#else
        return __builtin_clz(x);
#endif
    }

    static int popCount(unsigned x) {
#ifndef __GNUC__
        return (int)__popcnt(x);
        /*
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return x & 0x0000003F;
        */
#else
        return __builtin_popcount(x);
#endif
    }
};
