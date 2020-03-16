//
// Created by PCHEN on 2020-03-17.
//

#ifndef INCLUDED_PR_COUNTDOWNLATCH_H
#define INCLUDED_PR_COUNTDOWNLATCH_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_countdownlatch_s pr_countdownlatch_t;

////////////////////////////////////////////////////////////////////////////////
pr_countdownlatch_t* pr_countdownlatch_new(int count);

void pr_countdownlatch_delete(pr_countdownlatch_t**);

void pr_countdownlatch_countdown(pr_countdownlatch_t*);

void pr_countdownlatch_await(pr_countdownlatch_t*);

#endif //INCLUDED_PR_COUNTDOWNLATCH_H
