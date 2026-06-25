#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000

// Huffman node
struct Node {
    char ch;
    int freq;
    struct Node *left, *right;
};

// -------- Huffman code table ----------
char codes[256][MAX];  // store Huffman codes for each character

struct Node* createNode(char ch, int freq) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->ch = ch;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

void sortNodes(struct Node* arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i]->freq > arr[j]->freq) {
                struct Node* t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
        }
    }
}

// store codes instead of only printing
void buildCodes(struct Node* root, char code[], int top) {
    if (root->left) {
        code[top] = '0';
        buildCodes(root->left, code, top + 1);
    }

    if (root->right) {
        code[top] = '1';
        buildCodes(root->right, code, top + 1);
    }

    if (!root->left && !root->right) {
        code[top] = '\0';
        strcpy(codes[(unsigned char)root->ch], code);
    }
}

// print ASCII (8-bit binary)
void printASCII(char c) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (c >> i) & 1);
    }
}

// encode full text using Huffman
void printHuffmanEncoded(char text[]) {
    printf("\nHuffman Encoded Text:\n");
    for (int i = 0; text[i] != '\0'; i++) {
        printf("%s ", codes[(unsigned char)text[i]]);
    }
    printf("\n");
}

int main() {
    char text[MAX];
    int freq[256] = {0};

    printf("Enter text: ");
    fgets(text, MAX, stdin);
    text[strcspn(text, "\n")] = '\0';

    int len = strlen(text);

    // frequency count
    for (int i = 0; i < len; i++) {
        freq[(unsigned char)text[i]]++;
    }

    struct Node* nodes[MAX];
    int size = 0;

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            nodes[size++] = createNode((char)i, freq[i]);
        }
    }

    if (size == 0) return 0;

    // build tree
    while (size > 1) {
        sortNodes(nodes, size);

        struct Node* left = nodes[0];
        struct Node* right = nodes[1];

        struct Node* parent = createNode('$', left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        nodes[0] = parent;
        for (int i = 1; i < size - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        size--;
    }

    char code[MAX];

    buildCodes(nodes[0], code, 0);

    // -------- OUTPUT SECTION --------
    printf("\nCharacter | ASCII (8-bit) | Huffman Code\n");
    printf("----------------------------------------\n");

    for (int i = 0; i < len; i++) {
        char c = text[i];

        printf("    %c     | ", c);
        printASCII(c);
        printf(" | %s\n", codes[(unsigned char)c]);
    }

    printHuffmanEncoded(text);

    return 0;
}