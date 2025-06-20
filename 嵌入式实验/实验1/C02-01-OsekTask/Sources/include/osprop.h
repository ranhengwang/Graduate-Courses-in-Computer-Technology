#ifndef OCC_OSPROP_H
#define OCC_OSPROP_H

#define CONFIG_OSEK_OCC


#define		OCC_HCS12X

#define		OCC_EXTSTATUS

//conformance class
#define		OCC_ECC1

#define		OCC_ORTIDEBUGLEVEL		

#define		OCC_STKCHECK



//ISR numbers
#define		OCC_ISRLEVELNUM		128

//interrupt stack size
#define		OCC_ISRSTACKSIZE		1024



#define		OCC_NAPPMODES		1



#define		OCC_ISR2



//scheduling policy
#define		OCC_FULLPREEMPT


//task quantity (including the idle task)
#define		OCC_NTSKS		19

//maximum ready tasks 
#define		OCC_TOTALACTIVENUMBER		19



//COUNTER quantity
#define		OCC_NCTRS		1
#define		OCC_COUNTER



//ALARM quantity
#define		OCC_NALMS		1
#define		OCC_ALARM


#define		OCC_ALMACTIVTASK



#define	OCC_LOCALMESSAGESONLY

#define	OCC_SIGACTIVTASK


#if defined (OCC_LOCALMESSAGESONLY)
//MESSAGE quantity
#define		OCC_NMSGS		2

#define		OCC_NFLAGS		0

//quantity of queues
#define		OCC_NQUEMSGS		0


#endif  /* OCC_LOCALMESSAGESONLY */




#endif/*	ifndef OCC_OSPROP_H	*/



