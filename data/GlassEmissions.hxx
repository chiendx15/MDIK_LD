
#pragma once

#include "Mocha.type.hxx"

#define GLASS_COUNT_YEAR	30
#define STANDARD_YEAR		2021

typedef struct {
	int		nIn;
	int		nOut;
} T_PRODUCTION_COUNT;

typedef struct {
	T_PRODUCTION_COUNT	count;
} T_TIME_EMISSION;

typedef struct {
	T_TIME_EMISSION		h[24];
	T_PRODUCTION_COUNT	count;
} T_DAY_EMISSION;

typedef struct {
	T_DAY_EMISSION		d[31];
	T_PRODUCTION_COUNT	count;
} T_MONTH_EMISSION;

typedef struct {
	T_MONTH_EMISSION	m[12];
	T_PRODUCTION_COUNT	count;
} T_YEAR_EMISSION;

typedef struct {
	T_YEAR_EMISSION		y[GLASS_COUNT_YEAR];
} T_GLASS_EMISSIONS;