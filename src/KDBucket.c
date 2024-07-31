#include "../include/KDBucket.h"
#include <float.h>

KDNode* initKDNode(Vector vec, INFO_TYPE information) {
    KDNode* node = (KDNode*)malloc(sizeof(KDNode));
    node->information = information;
    node->vec = vec;
    node->left = NULL;
    node->right = NULL;
    node->initalized = 0;
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
    }
    free(node);
}
KDNode* recInsert(KDNode* root, KDNode* node, int depth, int dim) {
    if (!root) {
        root = node;
        return root;
    }

    int cd = depth % dim;

    if (node->vec.data[cd] < root->vec.data[cd]) {
        root->left = recInsert(root->left, node, depth+1, dim);
    } else {
        root->right = recInsert(root->right, node, depth+1, dim);
    }

    return root;
}
void insertKDNode(KDBucket* bucket, KDNode* node) {
    if (bucket->dim != node->vec.dim) {
        printf("\nNODE IGNORED: INVALID VECTOR DIMENSION\n");
        return;
    }
    bucket->root = recInsert(bucket->root, node, 0, bucket->dim);
    bucket->size++;
}
KDNode* recDelete(KDNode* root, KDNode* node, int depth, int dim) {
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
}
void recNN(KDNode* root, Vector* v, KDNode** best, double* bestdist, int depth) {
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