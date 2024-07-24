#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100


struct MinHeapNode {
    char data;
    unsigned frequency;
    struct MinHeapNode *left, *right;
};


struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode **array;
};

struct MinHeapNode* newNode(char data, unsigned frequency) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->frequency = frequency;
    return temp;
}


struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}


void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
        smallest = right;
    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}


struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->frequency < minHeap->array[(i - 1) / 2]->frequency) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}


void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}


int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}


struct MinHeap* createAndBuildMinHeap(char data[], int frequency[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], frequency[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}


struct MinHeapNode* buildHuffmanTree(char data[], int frequency[], int size) {
    struct MinHeapNode *left, *right, *top;
   
    struct MinHeap* minHeap = createAndBuildMinHeap(data, frequency, size);
  
    while (!isSizeOne(minHeap)) {
       
        left = extractMin(minHeap);
        right = extractMin(minHeap);
     
        top = newNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
 
    return extractMin(minHeap);
}


void printArr(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

void printCodes(struct MinHeapNode* root, int arr[], int top) {

    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
   
    if (isLeaf(root)) {
        printf("%c: ", root->data);
        printArr(arr, top);
    }
}

void HuffmanCodes(char data[], int frequency[], int size) {
  
    struct MinHeapNode* root = buildHuffmanTree(data, frequency, size);

    int arr[MAX_TREE_HT], top = 0;
    printCodes(root, arr, top);
}



void compressFile(char* input_file, char* output_file) {
    FILE* input_fp = fopen(input_file, "r");
    if (input_fp == NULL) {
        printf("Error: Unable to open input file.\n");
        exit(1);
    }
    FILE* output_fp = fopen(output_file, "wb");
    if (output_fp == NULL) {
        printf("Error: Unable to open output file.\n");
        exit(1);
    }

  
    int frequency[256] = {0}; 
    int c;
    while ((c = fgetc(input_fp)) != EOF) {
        frequency[c]++;
    }

    fwrite(frequency, sizeof(int), 256, output_fp);


    char characters[256];
    int frequencies[256];
    int count = 0;
    for (int i = 0; i < 256; i++) {
        if (frequency[i] > 0) {
            characters[count] = (char)i;
            frequencies[count] = frequency[i];
            count++;
        }
    }

    HuffmanCodes(characters, frequencies, count);

    fclose(input_fp);
    fclose(output_fp);
}


void decompressFile(char* input_file, char* output_file) {
    FILE* input_fp = fopen(input_file, "rb");
    if (input_fp == NULL) {
        printf("Error: Unable to open input file.\n");
        exit(1);
    }
    FILE* output_fp = fopen(output_file, "w");
    if (output_fp == NULL) {
        printf("Error: Unable to open output file.\n");
        exit(1);
    }


    int frequency[256] = {0};
    fread(frequency, sizeof(int), 256, input_fp);


    char characters[256];
    int frequencies[256];
    int count = 0;
    for (int i = 0; i < 256; i++) {
        if (frequency[i] > 0) {
            characters[count] = (char)i;
            frequencies[count] = frequency[i];
            count++;
        }
    }


    HuffmanCodes(characters, frequencies, count);

    fclose(input_fp);
    fclose(output_fp);
}

long int getFileSize(char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }
    fseek(fp, 0L, SEEK_END);
    long int size = ftell(fp);
    fclose(fp);
    return size;
}

int main() {
    char* input_file = "input.txt";
    char* compressed_file = "compressed.bin";

    compressFile(input_file, compressed_file);

 
    long int original_size = getFileSize(input_file);
    long int compressed_size = getFileSize(compressed_file);
    printf("Size of the original file: %ld bytes\n", original_size);
    printf("Size of the compressed file: %ld bytes\n", compressed_size);

    return 0;
}
