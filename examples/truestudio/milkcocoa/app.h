#ifndef _APP_H_
#define _APP_H_

#define TASKPRI_MAIN		4
#define TASKPRI_TIMELAPSE	1
#define TASKPRI_CHARLCD		TMAX_TPRI

#define TASKSTACKSIZE		(1024)

#ifndef KMM_SIZE
#define	KMM_SIZE	(TASKSTACKSIZE * 32)	/* カーネルが割り付ける */
#endif /* KMM_SIZE */						/* メモリ領域のサイズ */

#ifdef __cplusplus
extern "C" {
#endif

extern void task_main(intptr_t exinf);

#ifdef __cplusplus
}
#endif

#endif /* _APP_H_ */
