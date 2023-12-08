#pragma once
#include "GestureWrapper.h"
#include <vector>

using namespace std;

typedef vector<GestureWrapper*> GLIST;

class GestureDetection
{
public:
    GestureDetection(void);
    ~GestureDetection(void);

    //************************************
    // Desc:      �����Ҫ������������
    // Method:    AddGesture
    // FullName:  GestureDetection::AddGesture
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: GestureType type ��������
    //************************************
    void AddGesture(GestureType type);

    //************************************
    // Desc:      �������е�����
    // Method:    UpdateAllGesture
    // FullName:  GestureDetection::UpdateAllGesture
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: Joint * pJoints ������
    //************************************
    void UpdateAllGesture(Joint* pJoints);


    //************************************
    // Desc:      ����
    // Method:    RestAll
    // FullName:  GestureDetection::RestAll
    // Access:    public 
    // Returns:   void
    // Qualifier:
    //************************************
    void RestAll();

    // ʶ�����ص�����
    GestureRecognized fGestureRecongnized;

private:
    // �����б�
    GLIST* pGestureList; 
};

