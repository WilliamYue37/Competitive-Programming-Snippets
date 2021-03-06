struct Node {
    int key, priority;
    Node *l, *r;
    int sz; long long sum; //range queries
    bool flip; //lazy update

    Node(int key) {
        this->key = key; priority = rand() + (rand() << 15); l = r = NULL;
        sz = 1; sum = key; flip = false;
    }
};

int getSize(Node* t) {return t != NULL ? t->sz : 0;}
long long getSum(Node* t) {return t != NULL ? t->sum : 0;}

Node* push(Node* t) {
    if (t == NULL || !t->flip) return t;
    swap(t->l, t->r);
    t->flip = false;
    if (t->l != NULL) t->l->flip ^= 1;
    if (t->r != NULL) t->r->flip ^= 1;
    return t;
}

Node* pull(Node* t) {
    assert(!t->flip);
    push(t->l), push(t->r);
    t->sz = 1 + getSize(t->l) + getSize(t->r);
    t->sum = t->key + getSum(t->l) + getSum(t->r);
    return t;
}

// >= v goes to the right
pair<Node*, Node*> split(Node* t, int key) {
    if (t == NULL) return {t, t};
    push(t);
    if (t->key >= key) {
        auto p = split(t->l, key); t->l = p.second;
        return {p.first, pull(t)};
    }
    else {
        auto p = split(t->r, key); t->r = p.first;
        return {pull(t), p.second};
    }
}

pair<Node*, Node*> splitSize(Node* t, int sz) {
    if (t == NULL) return {t, t};
    push(t);
    if (getSize(t->l) >= sz) {
        auto p = splitSize(t->l, sz); t->l = p.second;
        return {p.first, pull(t)};
    }
    else {
        auto p = splitSize(t->r, sz - getSize(t->l) - 1); t->r = p.first;
        return {pull(t), p.second};
    }
}

//all keys in left must be < then all keys int right
Node* merge(Node* left, Node* right) {
    if (left == NULL || right == NULL) return left != NULL ? left : right;
    push(left), push(right);
    Node* t;
    if (left->priority > right->priority) left->r = merge(left->r, right), t = left;
    else right->l = merge(left, right->l), t = right;
    return pull(t);
}

Node* insert(Node* t, int key) {
    auto a = split(t, key), b = split(a.second, key + 1);
    return merge(a.first, merge(new Node(key), b.second));
}

Node* del(Node* t, int key) {
    auto a = split(t, key), b = split(a.second, key + 1);
    return merge(a.first, b.second);
}

bool contains(Node* t, int key) {
    if (t == NULL) return false; if (t->key == key) return true;
    return contains(t->l, key) || contains(t->r, key);
}
