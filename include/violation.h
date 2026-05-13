#ifndef VIOLATION_H
#define VIOLATION_H

/**
 * @file violation.h
 * @brief Violation management module - Stories 3.1, 3.2, and 3.3.
 *
 * Handles recording violations, penalty calculation, threshold checks,
 * member self-service views, and admin payment marking.
 */

#include "types.h"

/* ============================================================
 * Story 3.1 - Record Violation
 * ============================================================ */

int violationRecord(AppDatabase *db);

/* ============================================================
 * Story 3.2 - Out CLB Warning & Enforcement
 * ============================================================ */

int violationCheckOutThreshold(AppDatabase *db, Member *member);
void violationCheckAllOutClb(AppDatabase *db);

/* ============================================================
 * Story 3.3 - Mark Paid & View Own Violations/Fines
 * ============================================================ */

void violationViewOwn(AppDatabase *db);
void violationViewFines(AppDatabase *db);
int violationMarkPaid(AppDatabase *db);

#endif /* VIOLATION_H */
