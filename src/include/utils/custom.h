/* -------------------------------------------------------------------------
 *
 * custom.h
 *    Misc routines for custom-scan providers
 *
 * Portions Copyright (c) 1996-2016, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * -------------------------------------------------------------------------
 */
#ifndef CUSTOM_H
#define CUSTOM_H
#include "access/parallel.h"
#include "commands/explain.h"
#include "nodes/plannodes.h"
#include "nodes/execnodes.h"
#include "nodes/relation.h"
#include "storage/shm_toc.h"

/*
 * Flags for CustomPath nodes
 */
#define CUSTOMPATH_SUPPORT_BACKWARD_SCAN	0x0001
#define CUSTOMPATH_SUPPORT_MARK_RESTORE		0x0002


#define CUSTOMNAME_MAX_LEN			64

/*
 * Definition of the CustomPathMethods
 */
typedef struct CustomPathMethods
{
	const char *CustomName;

	/* Convert Path to a Plan */
	struct Plan *(*PlanCustomPath) (PlannerInfo *root,
									RelOptInfo *rel,
									struct CustomPath *best_path,
									List *tlist,
									List *clauses,
									List *custom_plans);
} CustomPathMethods;

/*
 * Definition of the CustomScanMethods
 */
typedef struct CustomScanMethods
{
	const char *CustomName;

	/* Create execution state (CustomScanState) from a CustomScan plan node */
	Node	   *(*CreateCustomScanState) (struct CustomScan *cscan);
} CustomScanMethods;

/*
 * Definition of the CustomExecMethods
 */
typedef struct CustomExecMethods
{
	const char *CustomName;

	/* Executor methods: mark/restore are optional, the rest are required */
	void		(*BeginCustomScan) (struct CustomScanState *node,
												EState *estate,
												int eflags);
	TupleTableSlot *(*ExecCustomScan) (struct CustomScanState *node);
	void		(*EndCustomScan) (struct CustomScanState *node);
	void		(*ReScanCustomScan) (struct CustomScanState *node);
	void		(*MarkPosCustomScan) (struct CustomScanState *node);
	void		(*RestrPosCustomScan) (struct CustomScanState *node);
	/* Optional: parallel execution support */
	Size		(*EstimateDSMCustomScan) (struct CustomScanState *node,
											   struct ParallelContext *pcxt);
	void		(*InitializeDSMCustomScan) (struct CustomScanState *node,
												struct ParallelContext *pcxt,
														void *coordinate);
	void		(*InitializeWorkerCustomScan) (struct CustomScanState *node,
														 struct shm_toc *toc,
														   void *coordinate);
	/* Optional: print additional information in EXPLAIN */
	void		(*ExplainCustomScan) (struct CustomScanState *node,
												  List *ancestors,
												  struct ExplainState *es);
} CustomExecMethods;

/*
 * registration and lookup
 */
extern void RegisterCustomScanMethods(const CustomScanMethods *methods);

extern const CustomScanMethods *GetCustomScanMethods(const char *custom_name,
													 bool missing_ok);
#endif	/* CUSTOM_H */


