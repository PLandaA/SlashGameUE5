#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.0f, 12, FColor::Red, true);
#define DRAW_SPHERE_COLOR(Location,Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 8.0f, 12, Color, false, 5.f);

#define DRAW_SPHERE_SINGLEFRAME(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);
#define DRAW_LINE_SINGLEFRAME(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Cyan, false, -1.0f, 0, 1.0f);
#define DRAW_POINT_SINGLEFRAME(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Green, false, -1.f);
#define DRAW_SPHERE_SingleFrame(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);


#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Cyan, true, -1.0f, 0, 1.0f);
#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Green, true);

#define DRAW_VECTOR(StartLocation, EndLocation, Color) if (GetWorld()) \
    { \
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color , true, -1.0f, 0, 1.0f); \
        DrawDebugPoint(GetWorld(), EndLocation, 15.f, Color, true); \
    }
#define DRAW_VECTOR_SINGLEFRAME(StartLocation, EndLocation, Color) if (GetWorld()) \
    { \
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color , false, -1.0f, 0, 1.0f); \
        DrawDebugPoint(GetWorld(), EndLocation, 15.f, Color, false, -1.f); \
    }
