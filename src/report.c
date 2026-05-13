#include "report.h"
#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

static int countMemberViolations(const AppDatabase *db, const char *studentId) {
  int count = 0;

  for (int i = 0; i < db->violationCount; i++) {
    if (strcmp(db->violations[i].studentId, studentId) == 0) {
      count++;
    }
  }

  return count;
}

static void swapMemberPointers(const Member **a, const Member **b) {
  const Member *tmp = *a;
  *a = *b;
  *b = tmp;
}

static void sortMemberPointersByViolationCount(const AppDatabase *db,
                                               const Member *sorted[],
                                               int memberCount,
                                               int ascending) {
  for (int i = 0; i < memberCount - 1; i++) {
    int selected = i;
    int selectedCount =
        countMemberViolations(db, sorted[selected]->studentId);

    for (int j = i + 1; j < memberCount; j++) {
      int currentCount = countMemberViolations(db, sorted[j]->studentId);
      int shouldSelect = 0;

      if (ascending) {
        shouldSelect = currentCount < selectedCount;
      } else {
        shouldSelect = currentCount > selectedCount;
      }

      if (shouldSelect) {
        selected = j;
        selectedCount = currentCount;
      }
    }

    if (selected != i) {
      swapMemberPointers(&sorted[i], &sorted[selected]);
    }
  }
}

/* ============================================================
 * REPORT EXPORT FUNCTIONS
 * ============================================================ */

void reportTeamStats(const AppDatabase *db) {
  if (db == NULL) {
    return;
  }

  double collected[4] = {0.0, 0.0, 0.0, 0.0};
  double outstanding[4] = {0.0, 0.0, 0.0, 0.0};

  for (int i = 0; i < db->violationCount; i++) {
    const Violation *v = &db->violations[i];

    /* Find the team of the member who committed the violation */
    int team = -1;
    for (int j = 0; j < db->memberCount; j++) {
      if (strcmp(db->members[j].studentId, v->studentId) == 0) {
        team = db->members[j].team;
        break;
      }
    }

    if (team >= TEAM_ACADEMIC && team <= TEAM_MEDIA) {
      if (v->isPaid) {
        collected[team] += v->fine;
      } else {
        outstanding[team] += v->fine;
      }
    }
  }

  printf("\n--- THONG KE TIEN PHAT THEO BAN ---\n");
  printf("%-15s | %-15s | %-15s | %-15s\n", "Ban", "Da thu (VND)",
         "Con no (VND)", "Tong (VND)");
  printf("---------------------------------------------------------------------"
         "-\n");

  for (int team = TEAM_ACADEMIC; team <= TEAM_MEDIA; team++) {
    double total = collected[team] + outstanding[team];
    printf("%-15s | %15.0f | %15.0f | %15.0f\n", teamName(team),
           collected[team], outstanding[team], total);
  }
  printf("---------------------------------------------------------------------"
         "-\n");
}

void reportSortMembersByViolations(const AppDatabase *db) {
  if (db == NULL) {
    return;
  }

  if (db->memberCount == 0) {
    printf("[THONG BAO] Chua co thanh vien nao trong du lieu\n");
    return;
  }

  int choice;
  printf("\nSAP XEP THEO SO LAN VI PHAM\n");
  printf("1. Tang dan\n");
  printf("2. Giam dan\n");
  printf("0. Quay lai\n");
  printf("Nhap lua chon: ");

  if (readInt(&choice) != 1) {
    printf("[LOI] Lua chon khong hop le\n");
    return;
  }

  if (choice == 0) {
    return;
  }
  if (choice != 1 && choice != 2) {
    printf("[LOI] Lua chon khong hop le\n");
    return;
  }

  const Member *sorted[MAX_MEMBERS];
  for (int i = 0; i < db->memberCount; i++) {
    sorted[i] = &db->members[i];
  }

  sortMemberPointersByViolationCount(db, sorted, db->memberCount, choice == 1);

  printf("\nDANH SACH THANH VIEN THEO SO LAN VI PHAM\n");
  printf("+----------------------+------------+--------------+--------------+\n");
  printf("| Ho va ten            | MSSV       | Ban          | So vi pham   |\n");
  printf("+----------------------+------------+--------------+--------------+\n");

  for (int i = 0; i < db->memberCount; i++) {
    int violationCount = countMemberViolations(db, sorted[i]->studentId);
    printf("| %-20.20s | %-10.10s | %-12.12s | %-12d |\n",
           sorted[i]->fullName, sorted[i]->studentId, teamName(sorted[i]->team),
           violationCount);
  }

  printf("+----------------------+------------+--------------+--------------+\n");
  printf("Tong: %d thanh vien\n", db->memberCount);
}
