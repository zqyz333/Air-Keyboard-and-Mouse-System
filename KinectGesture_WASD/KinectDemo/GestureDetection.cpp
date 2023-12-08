#include "stdafx.h"
#include "GestureDetection.h"

void WINAPI OnGestureWrapperRecognized(GestureType type);

// ȫ������
GestureDetection* pGDGlobal = NULL;

GestureDetection::GestureDetection(void) : 
    pGestureList(NULL)
{
    pGDGlobal = this;
}


GestureDetection::~GestureDetection(void)
{
    if (pGDGlobal)
    {
        pGDGlobal = NULL;
    }

    if (pGestureList)
    {
        GLIST::const_iterator it;
        for (it = pGestureList->begin(); it != pGestureList->end(); it ++)
        {
            GestureWrapper* pWrapper = (*it);
            delete pWrapper;
            pWrapper = NULL;
        }
        delete pGestureList;
        pGestureList = NULL;
    }
}

void GestureDetection::AddGesture(GestureType type)
{
    if (!pGestureList)
    {
        pGestureList = new GLIST();
    }

    GestureWrapper* pWrapper = new GestureWrapper(type);
    // ���ʶ�����ص�
    pWrapper->fGestureRecognized = OnGestureWrapperRecognized;
    pGestureList->push_back(pWrapper);
}

void GestureDetection::UpdateAllGesture(Joint* pJoints)
{
    GLIST::const_iterator it;
    for (it = pGestureList->begin(); it != pGestureList->end(); it ++)
    {
        GestureWrapper* pWrapper = (*it);
        pWrapper->UpdateGesture(pJoints);
    }
}

void GestureDetection::RestAll()
{
    GLIST::const_iterator it;
    for (it = pGestureList->begin(); it != pGestureList->end(); it ++)
    {
        GestureWrapper* pWrapper = (*it);
        pWrapper->Reset();
    }
}

//************************************
// Desc:      ����ʶ�����ص�����
// Method:    OnGestureWrapperRecognized
// FullName:  OnGestureWrapperRecognized
// Access:    public 
// Returns:   void WINAPI
// Qualifier:
// Parameter: GestureType type ��������
//************************************
void WINAPI OnGestureWrapperRecognized(GestureType type)
{
    if (pGDGlobal && pGDGlobal->fGestureRecongnized)
    {
        pGDGlobal->fGestureRecongnized(type);
    }

    if (pGDGlobal)
    {
        pGDGlobal->RestAll();
    }
}

