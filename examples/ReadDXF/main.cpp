#include <iostream>
#include "dx_iface.h"
#include "dx_data.h"

#ifdef _WIN32
#include <windows.h> 
#endif 
#ifndef S_ISDIR
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif 

void PrintEntities(dx_ifaceBlock* block) {
    if (!block) return;

    for (auto* ent : block->ent) {
        if (!ent) continue;

        switch (ent->eType) {
        case DRW::POINT: {
            DRW_Point* pt = dynamic_cast<DRW_Point*>(ent);
            if (pt) {
                std::cout << "POINT: ("
                    << pt->basePoint.x << ", "
                    << pt->basePoint.y << ", "
                    << pt->basePoint.z << ")\n";
            }
            break;
        }
        case DRW::LINE: {
            DRW_Line* line = dynamic_cast<DRW_Line*>(ent);
            if (line) {
                std::cout << "LINE: ("
                    << line->basePoint.x << ", "
                    << line->basePoint.y << ", "
                    << line->basePoint.z << ") -> ("
                    << line->secPoint.x << ", "
                    << line->secPoint.y << ", "
                    << line->secPoint.z << ")\n";
            }
            break;
        }
        case DRW::CIRCLE: {
            DRW_Circle* circle = dynamic_cast<DRW_Circle*>(ent);
            if (circle) {
                std::cout << "CIRCLE: center=("
                    << circle->basePoint.x << ", "
                    << circle->basePoint.y << ", "
                    << circle->basePoint.z
                    << "), radius=" << circle->radious << "\n";
            }
            break;
        }
        case DRW::ARC: {
            DRW_Arc* arc = dynamic_cast<DRW_Arc*>(ent);
            if (arc) {
                std::cout << "ARC: center=("
                    << arc->basePoint.x << ", "
                    << arc->basePoint.y << ", "
                    << arc->basePoint.z
                    << "), radius=" << arc->radious
                    << ", start=" << arc->staangle
                    << ", end=" << arc->endangle << "\n";
            }
            break;
        }
        case DRW::LWPOLYLINE:
            std::cout << "Entity: LWPOLYLINE (vertices not shown)\n";
            break;
        default:
            std::cout << "Entity: UNKNOWN\n";
            break;
        }
    }
}

int main() {

#ifdef _WIN32 
    SetConsoleOutputCP(CP_UTF8);
#endif   
   
    std::string fileName = "./dxf/line.dxf";

	bool debug = false;
    dx_data fData;
    dx_iface* input = new dx_iface();
    bool badState = input->fileImport(fileName, &fData, debug);

    PrintEntities(fData.mBlock);

    return 0;
}