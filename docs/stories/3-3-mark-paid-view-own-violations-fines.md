# Story 3.3: Mark Paid & View Own Violations/Fines

Status: done

## Story

As a member,
I want to view my own violations and outstanding fines,
so that I know what I owe and my payment status.

## Acceptance Criteria

1. **Given** a member selects View Own Violations  
   **When** the system reads the current session MSSV  
   **Then** only violations for that MSSV are displayed  
   **And** each row includes reason, date, fine amount, and paid/unpaid state
2. **Given** a member selects View Own Fines  
   **When** the system calculates outstanding debt  
   **Then** each unpaid violation is listed individually  
   **And** the total unpaid amount is shown at the bottom
3. **Given** BCN selects Mark Paid and chooses a violation  
   **When** the violation is marked with `isPaid=1`  
   **Then** the owning member's `totalFine` is recalculated immediately  
   **And** both `violations.dat` and `members.dat` are saved

## Tasks / Subtasks

- [x] Add member-facing views for own violations and own unpaid fines in `src/violation.c`
- [x] Restrict member access strictly to the session MSSV
- [x] Implement BCN mark-paid flow with safe violation selection
- [x] Recalculate `member.totalFine` from unpaid violations after payment updates
- [x] Save updated violations and members data after successful marking
- [x] Format output clearly for terminal review and demo scenarios

## Dev Notes

- Session access must come from `auth_get_session()`; do not pass ad hoc identity state through menus.
- `totalFine` is derived state and must stay synchronized with unpaid violations after every payment mutation.
- This story mixes member and BCN behaviors; keep role checks explicit so members cannot access BCN-only actions.

### References

- Story definition: [Source: _bmad-output/planning-artifacts/epics.md#Story-33-Mark-Paid--View-Own-ViolationsFines]
- Requirement mapping for member and BCN flows: [Source: _bmad-output/planning-artifacts/architecture.md#Requirements-to-Structure-Mapping]
- totalFine recalculation rules: [Source: _bmad-output/planning-artifacts/architecture.md#totalFine-Update-Rules]
- Mark paid data flow: [Source: _bmad-output/planning-artifacts/architecture.md#Data-Flow--Mark-Paid-with-totalFine-update]

## Dev Agent Record

### Agent Model Used

gpt-5

### Completion Notes List

- Story prepared with strict self-service access control and BCN payment reconciliation requirements
- Wired member and BCN menus to the new Story 3.3 flows
- Enforced BCN-only access for mark-paid actions via session role checks
- Restored paid/unpaid status display in the member violation list
- Added rollback for mark-paid when member persistence fails after saving violations
- Aligned the same cross-file rollback pattern with Story 3.1 so violation writes and member writes now recover consistently

### File List

- `_bmad-output/implementation-artifacts/3-3-mark-paid-view-own-violations-fines.md`
- `include/violation.h`
- `src/main.c`
- `src/violation.c`
