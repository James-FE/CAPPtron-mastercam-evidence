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
for(int i=0;i<count;i++){auto op=std::make_unique<operation>();op->op_idn=ids[i];DB_LIST_ENT_PTR ep=nullptr;bool ok=false;operation_manager(op.get(),OPMGR_GET,&ep,&ok);fprintf(f,"%s{\"id\":%ld,\"get\":%s",i?",":"",ids[i],ok?"true":"false");if(ok)fprintf(f,",\"opcode\":%d,\"slot\":%d,\"toolNumber\":%ld,\"toolSlot\":%ld,\"entityCount\":%ld,\"boundaryCount\":%ld,\"dirty\":%s,\"nciStart\":%lld,\"nciEnd\":%lld",(int)op->opcode,op->slot,op->tl.tlno,op->tl.slot,op->db.ent_count,op->db.bnd_count,op->db.nci_flag?"true":"false",(long long)op->db.nci_fpos,(long long)op->db.nci_fpos_end);if(ok)fprintf(f,",\"feed\":%.17g,\"rpm\":%ld,\"planeId\":%d,\"workOffset\":%d,\"origin\":[%.17g,%.17g,%.17g]",op->tl.feed,op->tl.rpm,op->WCS.m_PlaneID,op->WCS.woff_n,op->WCS.origin[0],op->WCS.origin[1],op->WCS.origin[2]);fputs("}",f);}fputs("]",f);fclose(f);return 0;}
