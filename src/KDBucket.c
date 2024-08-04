#include "../include/KDBucket.h"
#include <float.h>

KDNode* initKDNode(Vector vec, INFO_TYPE information) {
    KDNode* node = (KDNode*)malloc(sizeof(KDNode));
    node->information = information;
    node->vec = vec;
    node->left = NULL;
    node->right = NULL;
    node->initalized = 1;
    return node;
}
void freeKDNode(KDNode* node) {
    if (!node) {
        free(node);
        return;
    }
    if (node->initalized) {
        if (node->vec.data) {
            free(node->vec.data);
        }
        if (node->left) {
            freeKDNode(node->left);
        }
        if (node->right) {
            freeKDNode(node->right);
        }
        if (node->information) {
            free(node->information);
        }
    }
    free(node);
}
static KDNode* recInsert(KDNode* root, KDNode* node, int depth, int dim) {
    if (!root) {
        root = node;
        printf("Inserting at root\n");
        return root;
    }

    int cd = depth % dim;

    if (node->vec.data[cd] < root->vec.data[cd]) {
        printf("inserting left of root\n");
        root->left = recInsert(root->left, node, depth+1, dim);
    } else {
        printf("Inserting right of root\n");
        root->right = recInsert(root->right, node, depth+1, dim);
    }

    return root;
}
void insertKDNode(KDBucket* bucket, KDNode* node) {
    if (bucket->dim != node->vec.dim) {
        printf("\nNODE IGNORED: INVALID VECTOR DIMENSION, %d vs %d\n", bucket->dim, node->vec.dim);
        return;
    }
    bucket->root = recInsert(bucket->root, node, 0, bucket->dim);
    bucket->size++;
}
static KDNode* recDelete(KDNode* root, KDNode* node, int depth, int dim) {
    if (!root) {
        return NULL;
    }
    int cd = depth % dim;

    if (node->vec.data[cd] < root->vec.data[cd]) {
        root->left = recDelete(root->left, node, depth+1, dim);
    } else if (node->vec.data[cd] > root->vec.data[cd]) {
        root->right = recDelete(root->right, node, depth+1, dim);
    } else {
        //Deleting current node
        if (!root->left) {
            return root->right;
        } else if (!root->right) {
            return root->left;
        }

        KDNode* temp = root->right;
        while (temp->left) {
            temp = temp->left;
        }
        //Minimum value in right tree

        root->vec.data = temp->vec.data;
        //Copy data to create new root from the leftmost

        root->right = recDelete(root->right, temp, depth+1, dim);
    }

    return root;
}
void deleteKDNode(KDBucket* bucket, KDNode* node) {
    bucket->root = recDelete(bucket->root, node, 0, bucket->dim);
    bucket->size-=1;
}
static void recNN(KDNode* root, Vector* v, KDNode** best, double* bestdist, int depth) {
    if (!root) {
        return;
    }

    double dist = distSquared(v, &root->vec);
    if (dist < *bestdist) {
        *bestdist = dist;
        *best = root;
    }

    int cd = depth % root->vec.dim;

    double diff = v->data[cd] - root->vec.data[cd];
    KDNode* near;
    KDNode* far;
    if (diff > 0) {
        near = root->right;
        far = root->left;
    } else {
        near = root->left;
        far = root->right;
    }

    recNN(near, v, best, bestdist, depth+1);

    if (diff * diff  < *bestdist) {
        recNN(far, v, best, bestdist, depth+1);
    }
}
KDNode* nearestNeighbor(KDBucket* bucket, Vector* v) {
    KDNode* best = NULL;
    double bestdist = DBL_MAX;
    recNN(bucket->root, v, &best, &bestdist, 0);
    return best;
}
static void recKNN(KDNode* root, Vector* v, KDNode*** best, double** bestdists, int depth, int k) {
    if (!root) {
        return;
    }

    double dist = distSquared(v, &root->vec);
    double worst = -1.0;
    int index;
    for (int i = 0; i < k; i ++) {
        if (worst < (*bestdists)[i]) {
            worst = (*bestdists)[i];
            index = i;
        }
    }
    if (worst > dist) {
        (*bestdists)[index] = dist;
        (*best)[index] = root;
    }

    int cd = depth % root->vec.dim;

    double diff = v->data[cd] - root->vec.data[cd];
    KDNode* near;
    KDNode* far;
    if (diff > 0) {
        near = root->right;
        far = root->left;
    } else {
        near = root->left;
        far = root->right;
    }

    recKNN(near, v, best, bestdists, depth+1, k);

    for (int i = 0; i < k; i++) {
        if (diff * diff  < (*bestdists)[i]) {
            recKNN(far, v, best, bestdists, depth+1, k);
            break;
        }
    }
}
KDNode** kNearestNeighbors(KDBucket* bucket, Vector* v, int k) {
    KDNode** bests = (KDNode**)malloc(k*sizeof(KDNode*));
    double* bestdists = (double*)malloc(k*sizeof(double));
    for (int i = 0; i < k; i++) {
        bestdists[i] = DBL_MAX;
    }
    recKNN(bucket->root, v, &bests, &bestdists, 0, k);
    int sorted = 0;
    while (!sorted) {
        sorted = 1;
        for (int i = 1; i < k; i++) {
            if (bestdists[i] < bestdists[i-1]) {
                double buffer = bestdists[i-1];
                KDNode* buffer_node = bests[i-1];
                bestdists[i-1] = bestdists[i];
                bests[i-1] = bests[i];
                bestdists[i] = buffer;
                bests[i] = buffer_node;
                sorted = 0;
            }
        }
    }
    free(bestdists);
    return bests;
}
static void recTraverse(KDNode* node, KDNode** nodes) {
    if (node) {
        recTraverse(node->left, nodes);
        int i = 0;
        while (nodes[i]) {
            i++;
        }
        nodes[i] = node;
        recTraverse(node->right, nodes);
    }
}
KDNode** getAllNodes(KDBucket* bucket) {
    KDNode** nodes = (KDNode**)malloc(bucket->size * sizeof(KDNode*));
    for (int i = 0; i < bucket->size; i++) {
        nodes[i] = NULL;
    }
    recTraverse(bucket->root, nodes);
    return nodes;
}
KDBucket* initKDBucket(int dim) {
    KDBucket* bucket = (KDBucket*)malloc(sizeof(KDBucket));
    bucket->size = 0;
    bucket->dim = dim;
    bucket->root = NULL;
    return bucket;
}
void freeKDBucket(KDBucket* bucket) {
    freeKDNode(bucket->root);
    free(bucket);
}