/*-------------------------------------------------------------------------
 *
 * nodeLimit.h
 *
 *
 *
 * Portions Copyright (c) 1996-2016, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/executor/nodeLimit.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODELIMIT_H
#define NODELIMIT_H

#include "nodes/execnodes.h"

extern LimitState *ExecInitLimit(Limit *node, EState *estate, int eflags);
extern TupleTableSlot *ExecLimit(LimitState *node);
extern void ExecEndLimit(LimitState *node);
extern void ExecReScanLimit(LimitState *node);

extern void pass_down_bound(LimitState *node, PlanState *child_node);

#endif   /* NODELIMIT_H */
