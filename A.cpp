#include <cstdio>
#include <iostream>
#define MAX_LEN 262192

using namespace std;

typedef struct segTree {
	int leftodd, rightodd, lefteven, righteven, intervaleven, intervalodd;
	int intervalxor;
} SegmentTree;

SegmentTree tree[MAX_LEN];

SegmentTree newNode();
int isEven(int value);
void build(int l, int r, int index);
void update(int p, int l, int r, int index, int val);
SegmentTree mergeinterval(SegmentTree t1, SegmentTree t2, int l, int r);
SegmentTree query(int start, int end, int l, int r, int index);
int n, q;
int arr[MAX_LEN >> 1];

int main() {
	int T, i, j = 1;
	scanf("%d", &T);
	while (j <= T) {
		scanf("%d%d", &n, &q);
		for (i = 0 ; i < n; i++) {
			scanf("%d", &arr[i]);
		}	
		build(0, n - 1, 1);
		printf("Case #%d:", j);
		while (q > 0) {
			int p, value;
			cin >> p >> value;
			update(p, 0, n - 1, 1, value);
			
			printf(" %d", query(0, n - 1, 0, n - 1, 1).intervaleven);	
			q--;
		}
		printf("\n");
		j++;
	}
	return 0;
}

SegmentTree newNode() {
	SegmentTree t;
	t.leftodd =  t.rightodd= t.lefteven= t.righteven= t.intervaleven = t.intervalodd = t.intervalxor = 0;
	return t;
}

SegmentTree mergeinterval(SegmentTree t1, SegmentTree t2, int l, int r) {
	SegmentTree t;
	// int mid = l + (r - l) / 2;
	t.intervalxor = (t1.intervalxor ^ t2.intervalxor);
	int curr = isEven(t.intervalxor);
	int left = isEven(t1.intervalxor);
	int right = isEven(t2.intervalxor);
	if (curr == 0) {
		t.intervalodd = t.leftodd = t.rightodd = r - l + 1;
		t.intervaleven = max(max(t1.righteven + t2.lefteven, t1.rightodd + t2.leftodd), max(t1.intervaleven, t2.intervaleven));
		if (left == 0) {
			t.lefteven = t1.leftodd + t2.leftodd;
		} else {
			t.lefteven = t1.lefteven + t2.lefteven;
		}
	
		if (right == 0) {
			t.righteven = t1.rightodd + t2.rightodd;
		} else {
			t.righteven = t2.righteven + t1.righteven;
		}
	} else {
		t.intervaleven = t.lefteven = t.righteven = r - l + 1;
		t.intervalodd = max(max(t1.rightodd + t2.lefteven, t1.righteven + t2.leftodd), max(t1.intervalodd, t2.intervalodd));
		if (left == 0) {
			t.leftodd = t1.leftodd + t2.lefteven;
		} else {
			t.leftodd = t1.lefteven + t2.leftodd;
		}
	
		if (right == 0) {
			t.rightodd = t1.righteven + t2.rightodd;
		} else {
			t.rightodd = t2.rightodd + t1.righteven;
		}
	}
	
	
	
	return t;
}

int isEven(int value) {
	int count = 0;
	while (value > 0) {
		if (value % 2 == 1) {
			count++;
		}
		value /= 2;
	}
	return (count % 2 == 0 ? 1 : 0);
}

void build(int l, int r, int index) {
	if (l > r) return;
	if (l == r) {
		tree[index].intervalxor = arr[l];
		int inte = isEven(arr[l]);
		tree[index].intervalodd = tree[index].leftodd = tree[index].rightodd = 1 - inte;
		tree[index].intervaleven = tree[index].lefteven = tree[index].righteven = inte;
		return;	
	}
	
	int mid = l + (r - l) / 2;
	build(l, mid, index * 2);
	build(mid + 1, r, index * 2 + 1);
	tree[index] = mergeinterval(tree[index * 2], tree[index * 2 + 1], l, r);
}


void update(int p, int l, int r, int index, int val) {
	if (l > r) return;
	if (l == r) {
		tree[index].intervalxor = val;
		int inte = isEven(val);
		tree[index].intervalodd = tree[index].leftodd = tree[index].rightodd = 1 - inte;
		tree[index].intervaleven = tree[index].lefteven = tree[index].righteven = inte;
		return;
	}
	
	int mid = l + (r - l) / 2;
	if (p <= mid) {
		update(p, l, mid, index * 2, val);
	} else {
		update(p, mid + 1, r, index * 2 + 1, val);
	}
	
	tree[index] = mergeinterval(tree[index * 2], tree[index * 2 + 1], l, r);
}

SegmentTree query(int start, int end, int l, int r, int index) {
	if (start > end || l > r) return newNode();
	if (start > r || l > end) return newNode();
	if (start <= l && r <= end) return tree[index];
	int mid = l +  (r - l) / 2 ;
	if (end <= mid) {
		return query(start, end, l, mid, index * 2);
	} else if (mid + 1 <= start) {
		return query(start, end, mid + 1, r, index * 2 + 1);
	}
	
	return mergeinterval(query(start, end, l, mid, index * 2), query(start, end, mid + 1, r, index * 2 + 1), l, r);
} 

