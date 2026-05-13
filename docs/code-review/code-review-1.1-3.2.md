# Code Review: Stories 1.1 - 3.2

Date: 2026-05-13
Reviewer: Codex
Scope: Story implementations `1.1` through `3.2`

## Outcome

Review still does **not** clear the full `1.1-3.2` range for promotion, but the prior `violationRecord()` persistence-drift issue has now been fixed.

## Findings

### High

1. `memberEdit()` can persist a partially updated member after role changes if saving later files fails.  
   Reference: `src/member.c:345-375`  
   Impact: `members.dat` can contain a new role and recalculated `totalFine` while `violations.dat` and `accounts.dat` still contain the old state.

2. `memberDelete()` deletes in memory and then saves `accounts`, `violations`, and `members` separately without rollback.  
   Reference: `src/member.c:431-475`  
   Impact: a failed second or third save can leave orphaned or half-deleted records across `.dat` files.

## Fixed Since Review

1. `violationRecord()` no longer leaves `violations.dat` and `members.dat` out of sync when the second save fails.  
   Current reference: `src/violation.c:112-178`  
   Change: the function now snapshots member state and restores `violations.dat` if `members.dat` cannot be saved.

## Recommendation

- Keep BMAD story progression unchanged until the persistence/rollback defects are fixed.
- Do not mark Stories `1.5` through `3.2` as done based on the current branch state.
- Story `3.1` no longer needs follow-up for the specific `violationRecord()`/`members.dat` drift issue.
- Remaining open items in this review are `memberEdit()` and `memberDelete()`.

## Verification

- Reviewed source in `src/auth.c`, `src/fileio.c`, `src/main.c`, `src/member.c`, `src/report.c`, `src/utils.c`, and `src/violation.c`
- Ran `mingw32-make`
