#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <rpc.h>
#include <objbase.h>
#include <cassert>
#ifndef ASSERT
#define ASSERT(expr) assert(expr)
#endif
#include <cstdio>
#include <memory>
#include "BaseOperationTypes_CH.h"
#include "AssocHok_CH.h"
#include "3DVarsDefines_CH.h"

#include "IOVars_CH.h"

#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
template<class TYPE,class ARG_TYPE> class CArray;
#include "Entity_CH.h"
// Exact public declaration from Core/Assoc_CH.h; avoid unrelated CString-dependent EntityMap include.
__declspec(dllimport) long GetEntityByID(long entIdn,ent& theEnt,bool* succf);
#include "Chain_CH.h"

__declspec(dllimport) std::vector<ent_tp> GetAllTpEntities(long operationIdn,ent& opEntStruct,bool updateOpEntCount);

extern "C" __declspec(dllexport) int ReadOps(const long* ids,int count,const wchar_t* output){
FILE* f=nullptr;if(_wfopen_s(&f,output,L"wx")||!f)return 91;fputs("[",f);
for(int i=0;i<count;i++){auto op=std::make_unique<operation>();op->op_idn=ids[i];DB_LIST_ENT_PTR ep=nullptr;bool ok=false;operation_manager(op.get(),OPMGR_GET,&ep,&ok);fprintf(f,"%s{\"id\":%ld,\"get\":%s",i?",":"",ids[i],ok?"true":"false");if(ok)fprintf(f,",\"opcode\":%d,\"slot\":%d,\"toolNumber\":%ld,\"toolSlot\":%ld,\"entityCount\":%ld,\"boundaryCount\":%ld,\"dirty\":%s,\"nciStart\":%lld,\"nciEnd\":%lld",(int)op->opcode,op->slot,op->tl.tlno,op->tl.slot,op->db.ent_count,op->db.bnd_count,op->db.nci_flag?"true":"false",(long long)op->db.nci_fpos,(long long)op->db.nci_fpos_end);if(ok)fprintf(f,",\"feed\":%.17g,\"rpm\":%ld,\"planeId\":%d,\"workOffset\":%d,\"origin\":[%.17g,%.17g,%.17g]",op->tl.feed,op->tl.rpm,op->WCS.m_PlaneID,op->WCS.woff_n,op->WCS.origin[0],op->WCS.origin[1],op->WCS.origin[2]);if(ok && op->opcode==61)fprintf(f,",\"finishStockX\":%.17g,\"finishStockZ\":%.17g",op->u.prm_lfin.stock_x,op->u.prm_lfin.stock_z);if(ok)fprintf(f,",\"useCss\":%s,\"surfaceFinishFeed\":%s,\"toolMetric\":%s,\"cornerRadius\":%.17g",op->tl.use_css?"true":"false",op->tl.surf_fin_feed?"true":"false",op->tl.mm?"true":"false",op->tl.crad);if(ok)fprintf(f,",\"insertRadius\":%.17g,\"cplaneId\":%d,\"tplaneId\":%d,\"tplaneOrigin\":[%.17g,%.17g,%.17g],\"tplaneMatrix\":[[%.17g,%.17g,%.17g],[%.17g,%.17g,%.17g],[%.17g,%.17g,%.17g]]",op->tl.ltool.radius,op->cpln.m_PlaneID,op->tpln.m_PlaneID,op->tpln.origin[0],op->tpln.origin[1],op->tpln.origin[2],op->tpln.m[0][0],op->tpln.m[0][1],op->tpln.m[0][2],op->tpln.m[1][0],op->tpln.m[1][1],op->tpln.m[1][2],op->tpln.m[2][0],op->tpln.m[2][1],op->tpln.m[2][2]);fputs("}",f);}fputs("]",f);fclose(f);return 0;}
extern "C" __declspec(dllexport) int ChangeOp(int field,double value){
 auto op=std::make_unique<operation>();op->op_idn=3; DB_LIST_ENT_PTR ep=nullptr;bool ok=false;
 operation_manager(op.get(),OPMGR_GET,&ep,&ok);if(!ok||op->opcode!=61)return 3;
 if(field==1)op->WCS.woff_n=(short)value;
 else if(field==2)op->WCS.origin[0]=value;
 else if(field==3)op->tl.feed=value;
 else if(field==4)op->tl.rpm=(long)value;
 else if(field==5)op->u.prm_lfin.stock_x=value;
 else if(field==6)op->u.prm_lfin.stock_z=value;
 else return 4;
 operation_manager(op.get(),OPMGR_REWRITE,&ep,&ok);return ok?0:5;
}

extern "C" __declspec(dllexport) int ChangeExtra(int id,int field,double value){
 auto op=std::make_unique<operation>();op->op_idn=id;DB_LIST_ENT_PTR ep=nullptr;bool ok=false;
 operation_manager(op.get(),OPMGR_GET,&ep,&ok);if(!ok||op->opcode!=61)return 3;
 if(field==1){operation_manager(op.get(),OPMGR_NCI_FLAG,&ep,&ok);return ok?0:5;}
 else if(field==2)op->tl.crad=value;
 else if(field==3)op->tl.ltool.radius=value;
 else if(field==4)op->tl.use_css=(value!=0);
 else if(field==5)op->tl.feed=value;
 else return 4;
 operation_manager(op.get(),OPMGR_REWRITE,&ep,&ok);return ok?0:5;
}

