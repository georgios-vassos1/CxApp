#ifndef REVIEW_H
#define REVIEW_H

typedef struct {
    char *movie_name;
    char *reviewer_name;
    char *review_text;
    float review_score;
} review_t;

// Function declaration
review_t* read_review(const char *filename);

#endif // REVIEW_H
