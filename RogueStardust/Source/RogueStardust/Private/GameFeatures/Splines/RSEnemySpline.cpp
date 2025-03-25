// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Splines/RSEnemySpline.h"

ARSEnemySpline::ARSEnemySpline() : Super()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SplineType = ERSSplineType::E_EnemySpline;
}