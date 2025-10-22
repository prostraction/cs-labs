#include <iostream>

int main() {
    int p, v;   // 1
    int q, m;   // 2
    std::cin >> p >> v;
    std::cin >> q >> m;

    int s = 0;
    if (v > 0) {
        s += 2 * v + 1;
    }
    if (m > 0) {
        s += 2 * m + 1;
    }

    int mpv = p - v; // [... ]
    int ppv = p + v; // [       ...]
    int mqm = q - m; // [... ]
    int pqm = q + m; // [       ...]

    int l1beg, l2beg, l1end, l2end;
    if (mpv > mqm) {
        l1beg = mqm;
        l1end = pqm;
        l2beg = mpv;
        l2end = ppv;
    } else {
        l1beg = mpv;
        l1end = ppv;
        l2beg = mqm;
        l2end = pqm;
    }

    if (l2end < l1end && l1beg < l2beg) {
        s = l1end - l1beg + 1;
    } else if (l1end >= l2beg) {
        s -= (l1end - l2beg + 1);
    }

    if (s > 0) {
        std::cout << s;
    } else {
        std::cout << 0;
    }
}