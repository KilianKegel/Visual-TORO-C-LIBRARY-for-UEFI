/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeCoreMemRealloc.c

Abstract:

    Toro C Library common memory suballocator

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

#define PBYTE unsigned char*
#define PAGESIZE 4096

void* _cdeCoreMemRealloc(
    CDE_APP_IF* pCdeAppIf,
    IN void* ptr,   /* input pointer for realloc */
    IN size_t size, /*  input size for realloc*/
    HEAPDESC* pHeapStart
);

typedef unsigned long long EFI_PHYSICAL_ADDRESS;

static void __insertFree(HEAPDESC* pThis,
    HEAPDESC* pSucc,
    HEAPDESC* pFree,
    HEAPDESC* pHeapStart,
    CDE_APP_IF* pCdeAppIf
) {

    pThis->pSucc = pFree;   //link forward
    pFree->pSucc = pSucc;   //link forward

    pSucc->pPred = pFree;   //link backward
    pFree->pPred = pThis;   //link backward

//  2. free that free block (fuse with  successing free block)

    pFree->qwMagic = FREEMEM;
    _cdeCoreMemRealloc(pCdeAppIf, &pFree[1], 0, pHeapStart);    // fuse with bounding free block //KG20160402
}

/** _cdeCoreMemRealloc()

Synopsis

    void* _cdeCoreMemRealloc(
        CDE_APP_IF* pCdeAppIf,
        void* ptr,
        size_t size,
        HEAPDESC* pHeapStart
    )

Description

    Toro C Library common memory suballocator

Returns

    @param[in] CDE_APP_IF* pCdeAppIf
    @param[in] void* ptr
    @param[in] size_t size
    @param[in] HEAPDESC * pHeapStart

    @retval void * memory block
            NULL on FAIL

**/
void* _cdeCoreMemRealloc(
    CDE_APP_IF* pCdeAppIf,
    IN void* ptr,   /* input pointer for realloc */
    IN size_t size, /*  input size for realloc*/
    HEAPDESC* pHeapStart
)
{

    HEAPDESC* pThis = NULL/*,*pLast,*pPred*/, * pSucc, * pFree;
    unsigned int i;

    size_t nSizeW_AddHeapDesc;
    size_t nSizeWOAddHeapDesc;
#define HDX sizeof(HEAPDESC)
#define SIZ size
#define TBS ((unsigned)((PBYTE)&pThis->pSucc[0] - (PBYTE)&pThis[1]))    /* THIS BLOCK SIZE */
#define XBS (pThis->pSucc->qwMagic == FREEMEM ? ((unsigned)((PBYTE)&pThis->pSucc->pSucc[0] - (PBYTE)&pThis[1])) : 0) /* EXTENDABLE BLOCK SIZE */

    do {

        if (ptr != NULL) 
        {
            if (size != 0) 
            {
                static enum ACTION {
                    REA_DONOTHING,          /*((SIZ + HDX + 1) > TBS) && (SIZ <= TBS)   */
                    REA_SHRINK_W_DESC,      /*((SIZ + HDX + 1) <= TBS)                  */
                    REA_STRETCH_W_DESC,     /* ((SIZ + HDX + 1) <= XBS)                 */
                    REA_STRETCH_WO_DESC,    /* ((SIZ + HDX + 1) > XBS) && (SIZ <= XBS)  */
                    REA_MALLOC_MOVE,        /* ((SIZ + HDX + 1) > XBS) && (SIZ <= XBS)  */
                }action = REA_MALLOC_MOVE;

                pThis = &((HEAPDESC*)ptr)[-1];

                if (((SIZ + HDX + 1) > TBS) && (SIZ <= TBS))
                    action = REA_DONOTHING;
                else if ((SIZ + HDX + 1) <= TBS)
                    action = REA_SHRINK_W_DESC;     // shrink
                else if ((SIZ + HDX + 1) <= XBS)
                    action = REA_STRETCH_W_DESC;    // stretch
                else if (((SIZ + HDX + 1) > XBS) && (SIZ <= XBS))
                    action = REA_STRETCH_WO_DESC;   // stretch without adding a free block
                else
                    action = REA_MALLOC_MOVE;

                switch (action) 
                {
                    case REA_DONOTHING:
                        break;
                    case REA_SHRINK_W_DESC: {

                        pSucc = pThis->pSucc;
                        pFree = (HEAPDESC*)((PBYTE)&pThis[1] + size);

                        __insertFree(pThis, pSucc, pFree, pHeapStart, pCdeAppIf);

                        break;
                    }

                    case REA_STRETCH_W_DESC: {

                        pSucc = pThis->pSucc->pSucc;                                // the only thing we need to know
                        pFree = (HEAPDESC*)((PBYTE)&pThis[1] + size);

                        __insertFree(pThis, pSucc, pFree, pHeapStart, pCdeAppIf);

                        break;
                    }
                    case REA_STRETCH_WO_DESC: {
                        pSucc = pThis->pSucc->pSucc;
                        pThis->pSucc = pSucc;
                        pSucc->pPred = pThis;
                        break;
                    }
                    case REA_MALLOC_MOVE: {

                        unsigned char* pNewBuf, * pOldBuf;
                        pNewBuf = _cdeCoreMemRealloc(pCdeAppIf, 0, size, pHeapStart);   // allocate new size KG20160402
                        pOldBuf = (unsigned char*)&pThis[1];                            // get data area of ald buffer

                        if (pNewBuf != NULL) {
                            for (i = 0; i < TBS; i++)
                                pNewBuf[i] = pOldBuf[i];
                            _cdeCoreMemRealloc(pCdeAppIf, &pThis[1], 0, pHeapStart);        // fuse with bounding free block //KG20160402
                            pThis = &((HEAPDESC*)pNewBuf)[-1];
                        }
                        else {
                            pThis = 0;
                        }

                        break;
                    }
                }//switch (action)
                break;
            }
            //
            // free()
            //
            pThis = &((HEAPDESC*)ptr)[-1];
            //
            //todo: add validity check!!!
            //
            if (pThis->qwMagic == ALLOCMEM) {/*KG20170815_1 don't do anything if not own memory */
                pThis->qwMagic = FREEMEM;

                if (pThis->pSucc->qwMagic == FREEMEM/*ree*/) {                              // fuse with successor
                    pThis->pSucc = pThis->pSucc->pSucc;
                    pThis->pSucc->pPred = pThis;
                }

                if (pThis->pPred != NULL && pThis->pPred->qwMagic == FREEMEM) {         // fuse with predessor -> remove pThis
                    pThis->pPred->pSucc = pThis->pSucc;
                    pThis->pSucc->pPred = pThis->pPred;

                    pThis = pThis->pPred;
                }
                if (FALSE == pThis->fInalterable && (pThis->pPred != NULL && pThis->pPred->qwMagic == ENDOFMEM) && (pThis->pSucc->qwMagic == ENDOFMEM)) {   //free pages
                    pThis->pPred->pSucc = pThis->pSucc->pSucc;
                    if (NULL != pThis->pSucc->pSucc) {
                        pThis->pSucc->pSucc->pPred = pThis->pPred;
                    }
                    pCdeAppIf->pCdeServices->pMemFree(pCdeAppIf, pThis->PageBase, (unsigned int)pThis->Pages);
                }
            }/*KG20170815_1 don't do anything if not own memory */
            pThis = NULL;
            break;
        }
        else {  //if(ptr != NULL)
        //
        // malloc()
        //
            size = size == 0 ? 1 : size; //KG20170603 malloc(0),realloc(0,0) returns a pointer != 0

            pThis = pHeapStart;

            do {
                if (pThis->qwMagic == FREEMEM) {
                    nSizeWOAddHeapDesc = (PBYTE)(&pThis->pSucc[0]) - (PBYTE)(&pThis[1]);    // size with/out additional heap descriptor
                    nSizeW_AddHeapDesc = (PBYTE)(&pThis->pSucc[0]) - (PBYTE)(&pThis[2]);    // size with/    additional heap descriptor
                    nSizeWOAddHeapDesc = nSizeWOAddHeapDesc > (size_t)(&pThis->pSucc[0]) ? 0 : nSizeWOAddHeapDesc; // avoid negative numbers on unsigned
                    nSizeW_AddHeapDesc = nSizeW_AddHeapDesc > (size_t)(&pThis->pSucc[0]) ? 0 : nSizeW_AddHeapDesc; // avoid negative numbers on unsigned

                    if (nSizeWOAddHeapDesc >= size && nSizeW_AddHeapDesc < size) {  // exact match
                        pThis->qwMagic = ALLOCMEM;
                        break;
                    }

                    if (nSizeW_AddHeapDesc >= size) {  // can hold additional free HEAPDESC
                        pSucc = (HEAPDESC*)((PBYTE)&pThis[1] + size);

                        pThis->qwMagic = ALLOCMEM;
                        pSucc->qwMagic = FREEMEM;

                        pSucc->pSucc = pThis->pSucc;    //1
                        pSucc->pSucc->pPred = pSucc;    //2
                        pSucc->pPred = pThis;           //3
                        pThis->pSucc = pSucc;           //4

                        break;
                    }
                }

                if (NULL == pThis->pSucc) {

                    unsigned long long qwPages = ((size + 3 * HDX) % PAGESIZE) ? (unsigned long long)((size + 3 * HDX) / PAGESIZE) + 1 : (unsigned long long)((size + 3 * HDX) / PAGESIZE) + 0;
                    HEAPDESC* pTmpThis, * pTmpSucc, * pTmpEnd;
                    HEAPDESC* pPageBase;        //  W/A

                    pPageBase = qwPages >= CDE_MEMALLOC_PAGES_MAX ? NULL : pCdeAppIf->pCdeServices->pMemAlloc(pCdeAppIf, (unsigned int)qwPages);
                    if (pPageBase) {

                        pThis->pSucc = (HEAPDESC*)pPageBase;
                        pTmpThis = pThis->pSucc;                                                        // use tmp temporarily

                        pTmpEnd = (HEAPDESC*)((PBYTE)&pTmpThis[0] + (qwPages * PAGESIZE) - HDX);

                        pTmpSucc = (HEAPDESC*)((PBYTE)&pTmpThis[1] + size);

                        pTmpThis->qwMagic = FREEMEM;                                                    //set to ALLOCMEM during next turn
                        pTmpSucc->qwMagic = FREEMEM;
                        pTmpEnd->qwMagic = ENDOFMEM;

                        pTmpThis->PageBase = (EFI_PHYSICAL_ADDRESS)pPageBase;
                        pTmpThis->Pages = (unsigned int)qwPages;

                        pTmpThis->pPred = pThis;
                        pTmpThis->pSucc = pTmpSucc;
                        pTmpSucc->pPred = pTmpThis;
                        pTmpSucc->pSucc = pTmpEnd;
                        pTmpEnd->pPred = pTmpSucc;
                        pTmpEnd->pSucc = NULL;
                    }
                }

                if (NULL == pThis)
                    break;//while (pThis != NULL);
                pThis = pThis->pSucc;

            } while (pThis != NULL);
        }//if(ptr != NULL)
    } while (0);

    return pThis == NULL ? NULL : pThis + 1;
}