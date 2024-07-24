#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct item {
    int value;
    int weight;
} Item;

int compare(const void* a, const void* b) {
    const Item* item1 = (Item*)a;
    const Item* item2 = (Item*)b;

    double ratio1 = (double)item1->value / item1->weight;
    double ratio2 = (double)item2->value / item2->weight;

    return ratio1 > ratio2 ? -1 : 1;
}

double fractional_knapsack(int target, Item items[], int n) {
    qsort(items, n, sizeof(Item), compare);  

    int current_weight = 0;
    double final_value = 0.0;
    for (int i = 0; i < n; i++) {
        if (current_weight + items[i].weight <= target) {
            current_weight += items[i].weight;
            final_value = items[i].value;
        } else {
            int remaining = target - current_weight;
            final_value += ((double)items[i].value / items[i].weight) * remaining;
            break;  
        }
    }
    return final_value;
}

int main() {
    int n;
    scanf("%d", &n);

    Item items[n];
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &items[i].value, &items[i].weight);
    }

    int target;
    scanf("%d", &target);

    double result = fractional_knapsack(target, items, n);
    printf("%lf\n", result);

    return 0;
}
