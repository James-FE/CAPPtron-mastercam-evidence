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
for(int i=0;i<count;i++){auto op=std::make_unique<operation>();op->op_idn=ids[i];DB_LIST_ENT_PTR ep=nullptr;bool ok=false;operation_manager(op.get(),OPMGR_GET,&ep,&ok);fprintf(f,"%s{\"id\":%ld,\"get\":%s",i?",":"",ids[i],ok?"true":"false");if(ok)fprintf(f,",\"opcode\":%d,\"slot\":%d,\"toolNumber\":%ld,\"toolSlot\":%ld,\"entityCount\":%ld,\"boundaryCount\":%ld,\"dirty\":%s,\"nciStart\":%lld,\"nciEnd\":%lld",(int)op->opcode,op->slot,op->tl.tlno,op->tl.slot,op->db.ent_count,op->db.bnd_count,op->db.nci_flag?"true":"false",(long long)op->db.nci_fpos,(long long)op->db.nci_fpos_end);if(ok)fprintf(f,",\"feed\":%.17g,\"rpm\":%ld,\"planeId\":%d,\"workOffset\":%d,\"origin\":[%.17g,%.17g,%.17g]",op->tl.feed,op->tl.rpm,op->WCS.m_PlaneID,op->WCS.woff_n,op->WCS.origin[0],op->WCS.origin[1],op->WCS.origin[2]);if(ok && op->opcode==61)fprintf(f,",\"finishStockX\":%.17g,\"finishStockZ\":%.17g",op->u.prm_lfin.stock_x,op->u.prm_lfin.stock_z);if(ok)fprintf(f,",\"useCss\":%s,\"surfaceFinishFeed\":%s,\"toolMetric\":%s,\"cornerRadius\":%.17g",op->tl.use_css?"true":"false",op->tl.surf_fin_feed?"true":"false",op->tl.mm?"true":"false",op->tl.crad);fputs("}",f);}fputs("]",f);fclose(f);return 0;}
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

#include "Groups_CH.h"
__declspec(dllimport) op_group* GroupsMachineGroup(const op_group*);
namespace Cnc::OpMgr::OpGroupsAPI { __declspec(dllimport) const op_group* UpdateGroup(const op_group&); }
extern "C" __declspec(dllexport) int GroupPlane(int id,double x,const long* ids,int count,int axis){
 auto g=GroupsMachineGroup(nullptr);if(!g)return 2;auto cp=std::make_unique<op_group>(*g);
 cp->ogi.pg1.stock_view.m_PlaneID=(short)id;cp->ogi.pg1.stock_view.org[axis]=x;
 if(!Cnc::OpMgr::OpGroupsAPI::UpdateGroup(*cp))return 3;
 for(int i=0;i<count;i++){auto op=std::make_unique<operation>();op->op_idn=ids[i];DB_LIST_ENT_PTR ep=nullptr;bool ok=false;operation_manager(op.get(),OPMGR_GET,&ep,&ok);if(!ok||op->WCS.m_PlaneID!=1)return 4;op->WCS.m_PlaneID=(short)id;op->WCS.origin[axis]=x;operation_manager(op.get(),OPMGR_REWRITE,&ep,&ok);if(!ok)return 5;}return 0;
}
extern "C" __declspec(dllexport) int ReadGroupPlane(const wchar_t* path){auto g=GroupsMachineGroup(nullptr);if(!g)return 2;FILE*f=nullptr;if(_wfopen_s(&f,path,L"wx")||!f)return 91;auto&v=g->ogi.pg1.stock_view;fprintf(f,"{\"groupId\":%ld,\"planeId\":%d,\"origin\":[%.17g,%.17g,%.17g]}",g->grp_idn,v.m_PlaneID,v.org[0],v.org[1],v.org[2]);fclose(f);return 0;}

