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

#include "MachineDefApi/StorageControllerFactory_CH.h"
#include "MachineDefApi/IStockComponent_CH.h"
#include "MachineDefApi/IBarStockParametricGeometry_CH.h"
#include "MachineDefApi/IChuckComponent_CH.h"
#include "MachineDefApi/IChuckJawsComponent_CH.h"
namespace M=Cnc::Mastercam::Api::Machine;
extern "C" __declspec(dllexport) int ReadMachine(const wchar_t* path){
 FILE*f=nullptr;if(_wfopen_s(&f,path,L"wx")||!f)return 91;try{auto g=GroupsMachineGroup(nullptr);if(!g){fclose(f);return 2;}auto c=M::StorageControllerFactory().CreateDatabaseStorageController();auto init=c->Initialize(g->ogi.pg1.machine_ent_idn,false);fprintf(f,"initialize=%d code=%d\n",init->IsSuccess(),(int)init->GetResultCode());if(init->IsError()){fclose(f);return 3;}auto r=c->Read();fprintf(f,"read=%d code=%d\n",r->IsSuccess(),(int)r->GetResultCode());if(r->IsError()){fclose(f);return 4;}auto ctx=r->GetValue();auto machine=(*ctx)->GetMachine();
 std::function<void(M::IComponentPtr,int)> walk=[&](M::IComponentPtr p,int depth){fprintf(f,"depth=%d type=%d name=%ls\n",depth,(int)p->GetType(),p->GetName().c_str());auto chuck=std::dynamic_pointer_cast<M::IChuckComponent>(p);if(chuck)fprintf(f,"chuck jawsCount=%d jawsComponentPresent=%d\n",chuck->GetNumberOfJaws(),chuck->GetChuckJaws()?1:0);auto stock=std::dynamic_pointer_cast<M::IStockComponent>(p);if(stock){auto geo=stock->GetGeometry();auto bar=std::dynamic_pointer_cast<M::ICylinderParametricGeometry>(geo);fprintf(f,"stock cylinder=%d\n",bar?1:0);if(bar){auto pt=bar->GetBasePoint();auto axis=bar->GetAxisVector();fprintf(f,"radius=%.17g length=%.17g innerRadius=%.17g base=[%.17g,%.17g,%.17g] axis=[%.17g,%.17g,%.17g]\n",bar->GetRadius(),bar->GetLength(),bar->GetInnerRadius(),pt[0],pt[1],pt[2],axis[0],axis[1],axis[2]);}}if(depth<20)for(auto&ch:p->GetChildren())walk(ch,depth+1);};walk(machine->GetRoot(),0);fclose(f);return 0;}catch(const std::exception&e){fprintf(f,"exception=%s\n",e.what());fclose(f);return 9;}}
extern "C" __declspec(dllexport) int ChangeMachine(int field,double value,const wchar_t* path){
 FILE*f=nullptr;if(_wfopen_s(&f,path,L"wx")||!f)return 91;try{auto g=GroupsMachineGroup(nullptr);if(!g){fclose(f);return 2;}auto c=M::StorageControllerFactory().CreateDatabaseStorageController();auto init=c->Initialize(g->ogi.pg1.machine_ent_idn,false);if(init->IsError()){fclose(f);return 3;}auto r=c->Read();if(r->IsError()){fclose(f);return 4;}auto ctx=r->GetValue();auto m=(*ctx)->GetMachine();std::vector<M::IStockComponentPtr> stocks;std::vector<std::shared_ptr<M::IChuckJawsComponent>> jaws;
 std::function<void(M::IComponentPtr)> walk=[&](M::IComponentPtr p){if(auto q=std::dynamic_pointer_cast<M::IStockComponent>(p))stocks.push_back(q);if(auto q=std::dynamic_pointer_cast<M::IChuckJawsComponent>(p))jaws.push_back(q);for(auto&ch:p->GetChildren())walk(ch);};walk(m->GetRoot());fprintf(f,"stocks=%zu jaws=%zu\n",stocks.size(),jaws.size());
 if(field==4){if(jaws.size()!=1){fputs("FAILED: no unique native jaws component; legacy grip scalar is not a configured jaw\n",f);fclose(f);return 14;}jaws[0]->SetGripLength(value);}
 else{if(stocks.size()!=1){fclose(f);return 5;}auto bar=std::dynamic_pointer_cast<M::ICylinderParametricGeometry>(stocks[0]->GetGeometry());if(!bar){fclose(f);return 6;}if(field==1)bar->SetRadius(value/2);else if(field==2)bar->SetLength(value);else if(field==3){bar->SetInnerRadius(value/2);if(auto b=std::dynamic_pointer_cast<M::IBarStockParametricGeometry>(bar))b->SetIsHoleInStock(value>0);}else{fclose(f);return 7;}auto sr=stocks[0]->SetGeometry(bar);fprintf(f,"SetGeometry success=%d code=%d\n",sr->IsSuccess(),(int)sr->GetResultCode());if(sr->IsError()){fclose(f);return 8;}}
 auto result=c->Write(*m);fprintf(f,"Write success=%d code=%d\n",result->IsSuccess(),(int)result->GetResultCode());fprintf(f,"resultText=%ls\n",result->GetResultText().c_str());auto inner=result->GetInnerResult();if(inner)fprintf(f,"inner code=%d text=%ls\n",(int)inner->GetResultCode(),inner->GetResultText().c_str());int rc=result->IsSuccess()?0:10;fclose(f);return rc;}catch(const std::exception&e){fprintf(f,"exception=%s\n",e.what());fclose(f);return 9;}}

