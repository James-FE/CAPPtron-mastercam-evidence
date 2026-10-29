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

#include "Groups_CH.h"
// Exact SDK declarations from GUI/OpGroupsAPI_CH.h (avoid unrelated CString declarations).
__declspec(dllimport) op_group* GroupsMachineGroup(const op_group*);
#pragma comment(lib,"MCMachineDef.lib")
extern "C" __declspec(dllexport) int ReadStock(const wchar_t* output){
 
 auto g=GroupsMachineGroup(nullptr);if(!g)return 3;
 FILE* f=nullptr;if(_wfopen_s(&f,output,L"wx")||!f)return 91;
 auto& p=g->ogi.pg3;
 fprintf(f,"{\"groupId\":%ld,\"product\":%d,\"spindles\":[",g->grp_idn,(int)g->product);
 for(int i=0;i<2;i++){auto& s=p.stock_def[i];auto& c=p.chuck_def[i];auto ids=i?p.rstock_id:p.lstock_id;
 fprintf(f,"%s{\"index\":%d,\"boundaryIds\":[%ld,%ld],\"display\":%s,\"od\":%.17g,\"id\":%.17g,\"length\":%.17g,\"refZ\":%.17g,\"refAtMaxZ\":%s,\"holeInStock\":%s,\"useMargins\":%s,\"margins\":[%.17g,%.17g,%.17g,%.17g],\"chuckGripLength\":%.17g}",i?",":"",i,ids[0],ids[1],p.show_stock[i]?"true":"false",s.od,s.id,s.length,s.refZ,s.refAtMaxZ?"true":"false",s.holeInStock?"true":"false",s.useMargins?"true":"false",s.margins[0],s.margins[1],s.margins[2],s.margins[3],c.gripLength);
 }fputs("]}",f);fclose(f);return 0;
}
namespace Cnc::OpMgr::OpGroupsAPI { __declspec(dllimport) const op_group* UpdateGroup(const op_group&); }
extern "C" __declspec(dllexport) int Change(int field,double value){
 auto g=GroupsMachineGroup(nullptr);if(!g)return 3;
 auto copy=std::make_unique<op_group>(*g);
 auto& p=copy->ogi.pg3;
 if(field==1)p.stock_def[0].od=value;
 else if(field==2)p.stock_def[0].length=value;
 else if(field==3){p.stock_def[0].id=value;p.stock_def[0].holeInStock=true;}
 else if(field==4)p.chuck_def[0].gripLength=value;
 else return 4;
 return Cnc::OpMgr::OpGroupsAPI::UpdateGroup(*copy)?0:5;
}

