; ModuleID = './file2_origin_m_LLVMIR_LINK_NOOPT/./file2_origin_m_LLVMIR_LINK_NOOPT.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.__va_list_tag = type { i32, i32, ptr, ptr }

@totalOperations = dso_local global i32 0, align 4
@lastError = dso_local global i32 0, align 4
@logCount = dso_local global i32 0, align 4
@logMessages = dso_local global [100 x [256 x i8]] zeroinitializer, align 16
@.str = private unnamed_addr constant [54 x i8] c"ERROR: Invalid deductResource call (res=%d, amt=%.2f)\00", align 1
@resources = dso_local global [3 x double] zeroinitializer, align 16
@.str.1 = private unnamed_addr constant [39 x i8] c"Deducted %.2f from %s (new level=%.2f)\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"WATER\00", align 1
@.str.3 = private unnamed_addr constant [12 x i8] c"ELECTRICITY\00", align 1
@.str.4 = private unnamed_addr constant [5 x i8] c"FUEL\00", align 1
@.str.5 = private unnamed_addr constant [54 x i8] c"FAILED deduct: insufficient %s (have %.2f, need %.2f)\00", align 1
@.str.6 = private unnamed_addr constant [51 x i8] c"ERROR: Invalid addResource call (res=%d, amt=%.2f)\00", align 1
@.str.7 = private unnamed_addr constant [34 x i8] c"Added %.2f to %s (new level=%.2f)\00", align 1
@.str.8 = private unnamed_addr constant [65 x i8] c"convert: source and target are the same \E2\80\93 no conversion needed\00", align 1
@.str.9 = private unnamed_addr constant [78 x i8] c"Attempt conversion: %s -> %s : need %.2f of target, requires %.2f from source\00", align 1
@.str.10 = private unnamed_addr constant [72 x i8] c"Conversion successful: consumed %.2f of source, produced %.2f of target\00", align 1
@.str.11 = private unnamed_addr constant [43 x i8] c"Conversion failed during add - rolled back\00", align 1
@.str.12 = private unnamed_addr constant [42 x i8] c"Conversion failed: insufficient source %s\00", align 1
@.str.13 = private unnamed_addr constant [32 x i8] c"ObtainResource: need %.2f of %s\00", align 1
@.str.14 = private unnamed_addr constant [48 x i8] c"ObtainResource succeeded via conversion from %s\00", align 1
@.str.15 = private unnamed_addr constant [67 x i8] c"ObtainResource failed: no conversion path available for %.2f of %s\00", align 1
@.str.16 = private unnamed_addr constant [26 x i8] c"Request to use %.2f of %s\00", align 1
@.str.17 = private unnamed_addr constant [54 x i8] c"Deficit of %.2f, trying to obtain additional resource\00", align 1
@.str.18 = private unnamed_addr constant [65 x i8] c"Cannot use %.2f of %s \E2\80\93 insufficient after conversion attempts\00", align 1
@.str.19 = private unnamed_addr constant [21 x i8] c"Producing %.2f of %s\00", align 1
@.str.20 = private unnamed_addr constant [11 x i8] c"=== %s ===\00", align 1
@.str.21 = private unnamed_addr constant [5 x i8] c"%s:\0A\00", align 1
@.str.22 = private unnamed_addr constant [20 x i8] c"  Water:      %.2f\0A\00", align 1
@.str.23 = private unnamed_addr constant [20 x i8] c"  Electricity:%.2f\0A\00", align 1
@.str.24 = private unnamed_addr constant [20 x i8] c"  Fuel:       %.2f\0A\00", align 1
@.str.25 = private unnamed_addr constant [18 x i8] c"  Total ops:  %d\0A\00", align 1
@.str.26 = private unnamed_addr constant [37 x i8] c"\0A===== EVENT LOG (%d entries) =====\0A\00", align 1
@.str.27 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@.str.28 = private unnamed_addr constant [24 x i8] c"===== END OF LOG =====\0A\00", align 1
@.str.29 = private unnamed_addr constant [14 x i8] c"Initial State\00", align 1
@.str.30 = private unnamed_addr constant [61 x i8] c"\0A--- Demo 1: Heavy electricity usage (needs conversion) ---\0A\00", align 1
@.str.31 = private unnamed_addr constant [64 x i8] c"\0A--- Demo 2: Produce fuel and convert water to electricity ---\0A\00", align 1
@.str.32 = private unnamed_addr constant [84 x i8] c"\0A--- Demo 3: Massive water consumption (beyond reserves and conversion limits) ---\0A\00", align 1
@.str.33 = private unnamed_addr constant [51 x i8] c"\0A--- Demo 4: Interleaved production and usage ---\0A\00", align 1
@.str.34 = private unnamed_addr constant [12 x i8] c"Final State\00", align 1
@.str.35 = private unnamed_addr constant [22 x i8] c"\0ALast error code: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @logEvent(ptr noundef %0, ...) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca [1 x %struct.__va_list_tag], align 16
  store ptr %0, ptr %2, align 8
  %4 = load i32, ptr @logCount, align 4
  %5 = icmp sge i32 %4, 100
  br i1 %5, label %6, label %7

6:                                                ; preds = %1
  br label %19

7:                                                ; preds = %1
  %8 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %3, i64 0, i64 0
  call void @llvm.va_start.p0(ptr %8)
  %9 = load i32, ptr @logCount, align 4
  %10 = sext i32 %9 to i64
  %11 = getelementptr inbounds [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %10
  %12 = getelementptr inbounds [256 x i8], ptr %11, i64 0, i64 0
  %13 = load ptr, ptr %2, align 8
  %14 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %3, i64 0, i64 0
  %15 = call i32 @vsnprintf(ptr noundef %12, i64 noundef 256, ptr noundef %13, ptr noundef %14) #4
  %16 = getelementptr inbounds [1 x %struct.__va_list_tag], ptr %3, i64 0, i64 0
  call void @llvm.va_end.p0(ptr %16)
  %17 = load i32, ptr @logCount, align 4
  %18 = add nsw i32 %17, 1
  store i32 %18, ptr @logCount, align 4
  br label %19

19:                                               ; preds = %7, %6
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #1

; Function Attrs: nounwind
declare dso_local i32 @vsnprintf(ptr noundef, i64 noundef, ptr noundef, ptr noundef) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end.p0(ptr) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @deductResource(i32 noundef %0, double noundef %1) #0 {
  %3 = alloca i1, align 1
  %4 = alloca i32, align 4
  %5 = alloca double, align 8
  store i32 %0, ptr %4, align 4
  store double %1, ptr %5, align 8
  %6 = load i32, ptr %4, align 4
  %7 = icmp slt i32 %6, 0
  br i1 %7, label %14, label %8

8:                                                ; preds = %2
  %9 = load i32, ptr %4, align 4
  %10 = icmp sge i32 %9, 3
  br i1 %10, label %14, label %11

11:                                               ; preds = %8
  %12 = load double, ptr %5, align 8
  %13 = fcmp olt double %12, 0.000000e+00
  br i1 %13, label %14, label %17

14:                                               ; preds = %11, %8, %2
  store i32 1, ptr @lastError, align 4
  %15 = load i32, ptr %4, align 4
  %16 = load double, ptr %5, align 8
  call void (ptr, ...) @logEvent(ptr noundef @.str, i32 noundef %15, double noundef %16)
  store i1 false, ptr %3, align 1
  br label %64

17:                                               ; preds = %11
  %18 = load i32, ptr %4, align 4
  %19 = sext i32 %18 to i64
  %20 = getelementptr inbounds [3 x double], ptr @resources, i64 0, i64 %19
  %21 = load double, ptr %20, align 8
  %22 = load double, ptr %5, align 8
  %23 = fcmp oge double %21, %22
  br i1 %23, label %24, label %48

24:                                               ; preds = %17
  %25 = load double, ptr %5, align 8
  %26 = load i32, ptr %4, align 4
  %27 = sext i32 %26 to i64
  %28 = getelementptr inbounds [3 x double], ptr @resources, i64 0, i64 %27
  %29 = load double, ptr %28, align 8
  %30 = fsub double %29, %25
  store double %30, ptr %28, align 8
  %31 = load i32, ptr @totalOperations, align 4
  %32 = add nsw i32 %31, 1
  store i32 %32, ptr @totalOperations, align 4
  %33 = load double, ptr %5, align 8
  %34 = load i32, ptr %4, align 4
  %35 = icmp eq i32 %34, 0
  br i1 %35, label %36, label %37

36:                                               ; preds = %24
  br label %42

37:                                               ; preds = %24
  %38 = load i32, ptr %4, align 4
  %39 = icmp eq i32 %38, 1
  %40 = zext i1 %39 to i64
  %41 = select i1 %39, ptr @.str.3, ptr @.str.4
  br label %42

42:                                               ; preds = %37, %36
  %43 = phi ptr [ @.str.2, %36 ], [ %41, %37 ]
  %44 = load i32, ptr %4, align 4
  %45 = sext i32 %44 to i64
  %46 = getelementptr inbounds [3 x double], ptr @resources, i64 0, i64 %45
  %47 = load double, ptr %46, align 8
  call void (ptr, ...) @logEvent(ptr noundef @.str.1, double noundef %33, ptr noundef %43, double noundef %47)
  store i1 true, ptr %3, align 1
  br label %64

48:                                               ; preds = %17
  %49 = load i32, ptr %4, align 4
  %50 = icmp eq i32 %49, 0
  br i1 %50, label %51, label %52

51:                                               ; preds = %48
  br label %57

52:                                               ; preds = %48
  %53 = load i32, ptr %4, align 4
  %54 = icmp eq i32 %53, 1
  %55 = zext i1 %54 to i64
  %56 = select i1 %54, ptr @.str.3, ptr @.str.4
  br label %57

57:                                               ; preds = %52, %51
  %58 = phi ptr [ @.str.2, %51 ], [ %56, %52 ]
  %59 = load i32, ptr %4, align 4
  %60 = sext i32 %59 to i64
  %61 = getelementptr inbounds [3 x double], ptr @resources, i64 0, i64 %60
  %62 = load double, ptr %61, align 8
  %63 = load double, ptr %5, align 8
  call void (ptr, ...) @logEvent(ptr noundef @.str.5, ptr noundef %58, double noundef %62, double noundef %63)
  store i1 false, ptr %3, align 1
  br label %64

64:                                               ; preds = %57, %42, %14
  %65 = load i1, ptr %3, align 1
  ret i1 %65
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @addResource(i32 noundef %0, double noundef %1) #0 {
  %3 = alloca i1, align 1
  %4 = alloca i32, align 4
  %5 = alloca double, align 8
  store i32 %0, ptr %4, align 4
  store double %1, ptr %5, align 8
  %6 = load i32, ptr %4, align 4
  %7 = icmp slt i32 %6, 0
  br i1 %7, label %14, label %8

8:                                                ; preds = %2
  %9 = load i32, ptr %4, align 4
  %10 = icmp sge i32 %9, 3
  br i1 %10, label %14, label %11

11:                                               ; preds = %8
  %12 = load double, ptr %5, align 8
  %13 = fcmp olt double %12, 0.000000e+00
  br i1 %13, label %14, label %17

14:                                               ; preds = %11, %8, %2
  store i32 2, ptr @lastError, align 4
  %15 = load i32, ptr %4, align 4
  %16 = load double, ptr %5, align 8
  call void (ptr, ...) @logEvent(ptr noundef @.str.6, i32 noundef %15, double noundef %16)
  store i1 false, ptr %3, align 1
  br label %41

17:                                               ; preds = %11
  %18 = load double, ptr %5, align 8
  %19 = load i32, ptr %4, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds [3 x double], ptr @resources, i64 0, i64 %20
  %22 = load double, ptr %21, align 8
  %23 = fadd double %22, %18
  store double %23, ptr %21, align 8
  %24 = load i32, ptr @totalOperations, align 4
  %25 = add nsw i32 %24, 1
  store i32 %25, ptr @totalOperations, align 4
  %26 = load double, ptr %5, align 8
  %27 = load i32, ptr %4, align 4
  %28 = icmp eq i32 %27, 0
  br i1 %28, label %29, label %30

29:                                               ; preds = %17
  br label %35

30:                                               ; preds = %17
  %31 = load i32, ptr %4, align 4
  %32 = icmp eq i32 %31, 1
  %33 = zext i1 %32 to i64
  %34 = select i1 %32, ptr @.str.3, ptr @.str.4
  br label %35

35:                                               ; preds = %30, %29
  %36 = phi ptr [ @.str.2, %29 ], [ %34, %30 ]
  %37 = load i32, ptr %4, align 4
  %38 = sext i32 %37 to i64
  %39 = getelementptr inbounds [3 x double], ptr @resources, i64 0, i64 %38
  %40 = load double, ptr %39, align 8
  call void (ptr, ...) @logEvent(ptr noundef @.str.7, double noundef %26, ptr noundef %36, double noundef %40)
  store i1 true, ptr %3, align 1
  br label %41

41:                                               ; preds = %35, %14
  %42 = load i1, ptr %3, align 1
  ret i1 %42
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @convertResourceToGetTarget(i32 noundef %0, i32 noundef %1, double noundef %2, double noundef %3) #0 {
  %5 = alloca i1, align 1
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca double, align 8
  %9 = alloca double, align 8
  %10 = alloca double, align 8
  store i32 %0, ptr %6, align 4
  store i32 %1, ptr %7, align 4
  store double %2, ptr %8, align 8
  store double %3, ptr %9, align 8
  %11 = load i32, ptr %6, align 4
  %12 = load i32, ptr %7, align 4
  %13 = icmp eq i32 %11, %12
  br i1 %13, label %14, label %15

14:                                               ; preds = %4
  call void (ptr, ...) @logEvent(ptr noundef @.str.8)
  store i1 false, ptr %5, align 1
  br label %70

15:                                               ; preds = %4
  %16 = load double, ptr %8, align 8
  %17 = fcmp ole double %16, 0.000000e+00
  br i1 %17, label %18, label %19

18:                                               ; preds = %15
  store i1 true, ptr %5, align 1
  br label %70

19:                                               ; preds = %15
  %20 = load double, ptr %8, align 8
  %21 = load double, ptr %9, align 8
  %22 = fdiv double %20, %21
  store double %22, ptr %10, align 8
  %23 = load i32, ptr %6, align 4
  %24 = icmp eq i32 %23, 0
  br i1 %24, label %25, label %26

25:                                               ; preds = %19
  br label %31

26:                                               ; preds = %19
  %27 = load i32, ptr %6, align 4
  %28 = icmp eq i32 %27, 1
  %29 = zext i1 %28 to i64
  %30 = select i1 %28, ptr @.str.3, ptr @.str.4
  br label %31

31:                                               ; preds = %26, %25
  %32 = phi ptr [ @.str.2, %25 ], [ %30, %26 ]
  %33 = load i32, ptr %7, align 4
  %34 = icmp eq i32 %33, 0
  br i1 %34, label %35, label %36

35:                                               ; preds = %31
  br label %41

36:                                               ; preds = %31
  %37 = load i32, ptr %7, align 4
  %38 = icmp eq i32 %37, 1
  %39 = zext i1 %38 to i64
  %40 = select i1 %38, ptr @.str.3, ptr @.str.4
  br label %41

41:                                               ; preds = %36, %35
  %42 = phi ptr [ @.str.2, %35 ], [ %40, %36 ]
  %43 = load double, ptr %8, align 8
  %44 = load double, ptr %10, align 8
  call void (ptr, ...) @logEvent(ptr noundef @.str.9, ptr noundef %32, ptr noundef %42, double noundef %43, double noundef %44)
  %45 = load i32, ptr %6, align 4
  %46 = load double, ptr %10, align 8
  %47 = call zeroext i1 @deductResource(i32 noundef %45, double noundef %46)
  br i1 %47, label %48, label %59

48:                                               ; preds = %41
  %49 = load i32, ptr %7, align 4
  %50 = load double, ptr %8, align 8
  %51 = call zeroext i1 @addResource(i32 noundef %49, double noundef %50)
  br i1 %51, label %52, label %55

52:                                               ; preds = %48
  %53 = load double, ptr %10, align 8
  %54 = load double, ptr %8, align 8
  call void (ptr, ...) @logEvent(ptr noundef @.str.10, double noundef %53, double noundef %54)
  store i1 true, ptr %5, align 1
  br label %70

55:                                               ; preds = %48
  %56 = load i32, ptr %6, align 4
  %57 = load double, ptr %10, align 8
  %58 = call zeroext i1 @addResource(i32 noundef %56, double noundef %57)
  call void (ptr, ...) @logEvent(ptr noundef @.str.11)
  store i1 false, ptr %5, align 1
  br label %70

59:                                               ; preds = %41
  %60 = load i32, ptr %6, align 4
  %61 = icmp eq i32 %60, 0
  br i1 %61, label %62, label %63

62:                                               ; preds = %59
  br label %68

63:                                               ; preds = %59
  %64 = load i32, ptr %6, align 4
  %65 = icmp eq i32 %64, 1
  %66 = zext i1 %65 to i64
  %67 = select i1 %65, ptr @.str.3, ptr @.str.4
  br label %68

68:                                               ; preds = %63, %62
  %69 = phi ptr [ @.str.2, %62 ], [ %67, %63 ]
  call void (ptr, ...) @logEvent(ptr noundef @.str.12, ptr noundef %69)
  store i1 false, ptr %5, align 1
  br label %70

70:                                               ; preds = %68, %55, %52, %18, %14
  %71 = load i1, ptr %5, align 1
  ret i1 %71
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @obtainResource(i32 noundef %0, double noundef %1) #0 {
  %3 = alloca i1, align 1
  %4 = alloca i32, align 4
  %5 = alloca double, align 8
  %6 = alloca i32, align 4
  store i32 %0, ptr %4, align 4
  store double %1, ptr %5, align 8
  %7 = load double, ptr %5, align 8
  %8 = fcmp ole double %7, 0.000000e+00
  br i1 %8, label %9, label %10

9:                                                ; preds = %2
  store i1 true, ptr %3, align 1
  br label %62

10:                                               ; preds = %2
  %11 = load double, ptr %5, align 8
  %12 = load i32, ptr %4, align 4
  %13 = icmp eq i32 %12, 0
  br i1 %13, label %14, label %15

14:                                               ; preds = %10
  br label %20

15:                                               ; preds = %10
  %16 = load i32, ptr %4, align 4
  %17 = icmp eq i32 %16, 1
  %18 = zext i1 %17 to i64
  %19 = select i1 %17, ptr @.str.3, ptr @.str.4
  br label %20

20:                                               ; preds = %15, %14
  %21 = phi ptr [ @.str.2, %14 ], [ %19, %15 ]
  call void (ptr, ...) @logEvent(ptr noundef @.str.13, double noundef %11, ptr noundef %21)
  store i32 0, ptr %6, align 4
  br label %22

22:                                               ; preds = %47, %20
  %23 = load i32, ptr %6, align 4
  %24 = icmp slt i32 %23, 3
  br i1 %24, label %25, label %50

25:                                               ; preds = %22
  %26 = load i32, ptr %6, align 4
  %27 = load i32, ptr %4, align 4
  %28 = icmp eq i32 %26, %27
  br i1 %28, label %29, label %30

29:                                               ; preds = %25
  br label %47

30:                                               ; preds = %25
  %31 = load i32, ptr %6, align 4
  %32 = load i32, ptr %4, align 4
  %33 = load double, ptr %5, align 8
  %34 = call zeroext i1 @convertResourceToGetTarget(i32 noundef %31, i32 noundef %32, double noundef %33, double noundef 8.000000e-01)
  br i1 %34, label %35, label %46

35:                                               ; preds = %30
  %36 = load i32, ptr %6, align 4
  %37 = icmp eq i32 %36, 0
  br i1 %37, label %38, label %39

38:                                               ; preds = %35
  br label %44

39:                                               ; preds = %35
  %40 = load i32, ptr %6, align 4
  %41 = icmp eq i32 %40, 1
  %42 = zext i1 %41 to i64
  %43 = select i1 %41, ptr @.str.3, ptr @.str.4
  br label %44

44:                                               ; preds = %39, %38
  %45 = phi ptr [ @.str.2, %38 ], [ %43, %39 ]
  call void (ptr, ...) @logEvent(ptr noundef @.str.14, ptr noundef %45)
  store i1 true, ptr %3, align 1
  br label %62

46:                                               ; preds = %30
  br label %47

47:                                               ; preds = %46, %29
  %48 = load i32, ptr %6, align 4
  %49 = add nsw i32 %48, 1
  store i32 %49, ptr %6, align 4
  br label %22, !llvm.loop !4

50:                                               ; preds = %22
  %51 = load double, ptr %5, align 8
  %52 = load i32, ptr %4, align 4
  %53 = icmp eq i32 %52, 0
  br i1 %53, label %54, label %55

54:                                               ; preds = %50
  br label %60

55:                                               ; preds = %50
  %56 = load i32, ptr %4, align 4
  %57 = icmp eq i32 %56, 1
  %58 = zext i1 %57 to i64
  %59 = select i1 %57, ptr @.str.3, ptr @.str.4
  br label %60

60:                                               ; preds = %55, %54
  %61 = phi ptr [ @.str.2, %54 ], [ %59, %55 ]
  call void (ptr, ...) @logEvent(ptr noundef @.str.15, double noundef %51, ptr noundef %61)
  store i1 false, ptr %3, align 1
  br label %62

62:                                               ; preds = %60, %44, %9
  %63 = load i1, ptr %3, align 1
  ret i1 %63
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @useResource(i32 noundef %0, double noundef %1) #0 {
  %3 = alloca i1, align 1
  %4 = alloca i32, align 4
  %5 = alloca double, align 8
  %6 = alloca double, align 8
  store i32 %0, ptr %4, align 4
  store double %1, ptr %5, align 8
  %7 = load double, ptr %5, align 8
  %8 = fcmp ole double %7, 0.000000e+00
  br i1 %8, label %9, label %10

9:                                                ; preds = %2
  store i1 true, ptr %3, align 1
  br label %59

10:                                               ; preds = %2
  %11 = load double, ptr %5, align 8
  %12 = load i32, ptr %4, align 4
  %13 = icmp eq i32 %12, 0
  br i1 %13, label %14, label %15

14:                                               ; preds = %10
  br label %20

15:                                               ; preds = %10
  %16 = load i32, ptr %4, align 4
  %17 = icmp eq i32 %16, 1
  %18 = zext i1 %17 to i64
  %19 = select i1 %17, ptr @.str.3, ptr @.str.4
  br label %20

20:                                               ; preds = %15, %14
  %21 = phi ptr [ @.str.2, %14 ], [ %19, %15 ]
  call void (ptr, ...) @logEvent(ptr noundef @.str.16, double noundef %11, ptr noundef %21)
  %22 = load i32, ptr %4, align 4
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds [3 x double], ptr @resources, i64 0, i64 %23
  %25 = load double, ptr %24, align 8
  %26 = load double, ptr %5, align 8
  %27 = fcmp oge double %25, %26
  br i1 %27, label %28, label %32

28:                                               ; preds = %20
  %29 = load i32, ptr %4, align 4
  %30 = load double, ptr %5, align 8
  %31 = call zeroext i1 @deductResource(i32 noundef %29, double noundef %30)
  store i1 %31, ptr %3, align 1
  br label %59

32:                                               ; preds = %20
  %33 = load double, ptr %5, align 8
  %34 = load i32, ptr %4, align 4
  %35 = sext i32 %34 to i64
  %36 = getelementptr inbounds [3 x double], ptr @resources, i64 0, i64 %35
  %37 = load double, ptr %36, align 8
  %38 = fsub double %33, %37
  store double %38, ptr %6, align 8
  %39 = load double, ptr %6, align 8
  call void (ptr, ...) @logEvent(ptr noundef @.str.17, double noundef %39)
  %40 = load i32, ptr %4, align 4
  %41 = load double, ptr %6, align 8
  %42 = call zeroext i1 @obtainResource(i32 noundef %40, double noundef %41)
  br i1 %42, label %43, label %47

43:                                               ; preds = %32
  %44 = load i32, ptr %4, align 4
  %45 = load double, ptr %5, align 8
  %46 = call zeroext i1 @deductResource(i32 noundef %44, double noundef %45)
  store i1 %46, ptr %3, align 1
  br label %59

47:                                               ; preds = %32
  %48 = load double, ptr %5, align 8
  %49 = load i32, ptr %4, align 4
  %50 = icmp eq i32 %49, 0
  br i1 %50, label %51, label %52

51:                                               ; preds = %47
  br label %57

52:                                               ; preds = %47
  %53 = load i32, ptr %4, align 4
  %54 = icmp eq i32 %53, 1
  %55 = zext i1 %54 to i64
  %56 = select i1 %54, ptr @.str.3, ptr @.str.4
  br label %57

57:                                               ; preds = %52, %51
  %58 = phi ptr [ @.str.2, %51 ], [ %56, %52 ]
  call void (ptr, ...) @logEvent(ptr noundef @.str.18, double noundef %48, ptr noundef %58)
  store i32 3, ptr @lastError, align 4
  store i1 false, ptr %3, align 1
  br label %59

59:                                               ; preds = %57, %43, %28, %9
  %60 = load i1, ptr %3, align 1
  ret i1 %60
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @produceResource(i32 noundef %0, double noundef %1) #0 {
  %3 = alloca i1, align 1
  %4 = alloca i32, align 4
  %5 = alloca double, align 8
  store i32 %0, ptr %4, align 4
  store double %1, ptr %5, align 8
  %6 = load double, ptr %5, align 8
  %7 = fcmp olt double %6, 0.000000e+00
  br i1 %7, label %8, label %9

8:                                                ; preds = %2
  store i1 false, ptr %3, align 1
  br label %24

9:                                                ; preds = %2
  %10 = load double, ptr %5, align 8
  %11 = load i32, ptr %4, align 4
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %13, label %14

13:                                               ; preds = %9
  br label %19

14:                                               ; preds = %9
  %15 = load i32, ptr %4, align 4
  %16 = icmp eq i32 %15, 1
  %17 = zext i1 %16 to i64
  %18 = select i1 %16, ptr @.str.3, ptr @.str.4
  br label %19

19:                                               ; preds = %14, %13
  %20 = phi ptr [ @.str.2, %13 ], [ %18, %14 ]
  call void (ptr, ...) @logEvent(ptr noundef @.str.19, double noundef %10, ptr noundef %20)
  %21 = load i32, ptr %4, align 4
  %22 = load double, ptr %5, align 8
  %23 = call zeroext i1 @addResource(i32 noundef %21, double noundef %22)
  store i1 %23, ptr %3, align 1
  br label %24

24:                                               ; preds = %19, %8
  %25 = load i1, ptr %3, align 1
  ret i1 %25
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @printResources(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  call void (ptr, ...) @logEvent(ptr noundef @.str.20, ptr noundef %3)
  %4 = load ptr, ptr %2, align 8
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.21, ptr noundef %4)
  %6 = load double, ptr @resources, align 16
  %7 = call i32 (ptr, ...) @printf(ptr noundef @.str.22, double noundef %6)
  %8 = load double, ptr getelementptr inbounds ([3 x double], ptr @resources, i64 0, i64 1), align 8
  %9 = call i32 (ptr, ...) @printf(ptr noundef @.str.23, double noundef %8)
  %10 = load double, ptr getelementptr inbounds ([3 x double], ptr @resources, i64 0, i64 2), align 16
  %11 = call i32 (ptr, ...) @printf(ptr noundef @.str.24, double noundef %10)
  %12 = load i32, ptr @totalOperations, align 4
  %13 = call i32 (ptr, ...) @printf(ptr noundef @.str.25, i32 noundef %12)
  ret void
}

declare dso_local i32 @printf(ptr noundef, ...) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @printLogs() #0 {
  %1 = alloca i32, align 4
  %2 = load i32, ptr @logCount, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @.str.26, i32 noundef %2)
  store i32 0, ptr %1, align 4
  br label %4

4:                                                ; preds = %14, %0
  %5 = load i32, ptr %1, align 4
  %6 = load i32, ptr @logCount, align 4
  %7 = icmp slt i32 %5, %6
  br i1 %7, label %8, label %17

8:                                                ; preds = %4
  %9 = load i32, ptr %1, align 4
  %10 = sext i32 %9 to i64
  %11 = getelementptr inbounds [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %10
  %12 = getelementptr inbounds [256 x i8], ptr %11, i64 0, i64 0
  %13 = call i32 (ptr, ...) @printf(ptr noundef @.str.27, ptr noundef %12)
  br label %14

14:                                               ; preds = %8
  %15 = load i32, ptr %1, align 4
  %16 = add nsw i32 %15, 1
  store i32 %16, ptr %1, align 4
  br label %4, !llvm.loop !6

17:                                               ; preds = %4
  %18 = call i32 (ptr, ...) @printf(ptr noundef @.str.28)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store double 1.000000e+02, ptr @resources, align 16
  store double 5.000000e+01, ptr getelementptr inbounds ([3 x double], ptr @resources, i64 0, i64 1), align 8
  store double 8.000000e+01, ptr getelementptr inbounds ([3 x double], ptr @resources, i64 0, i64 2), align 16
  store i32 0, ptr @totalOperations, align 4
  store i32 0, ptr @lastError, align 4
  store i32 0, ptr @logCount, align 4
  call void @printResources(ptr noundef @.str.29)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @.str.30)
  %3 = call zeroext i1 @useResource(i32 noundef 1, double noundef 7.000000e+01)
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.31)
  %5 = call zeroext i1 @produceResource(i32 noundef 2, double noundef 3.000000e+01)
  %6 = call zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef 1, double noundef 1.500000e+01, double noundef 8.000000e-01)
  %7 = call i32 (ptr, ...) @printf(ptr noundef @.str.32)
  %8 = call zeroext i1 @useResource(i32 noundef 0, double noundef 1.200000e+02)
  %9 = call i32 (ptr, ...) @printf(ptr noundef @.str.33)
  %10 = call zeroext i1 @produceResource(i32 noundef 1, double noundef 1.000000e+01)
  %11 = call zeroext i1 @useResource(i32 noundef 2, double noundef 5.000000e+01)
  %12 = call zeroext i1 @produceResource(i32 noundef 0, double noundef 4.000000e+01)
  %13 = call zeroext i1 @useResource(i32 noundef 1, double noundef 2.500000e+01)
  call void @printResources(ptr noundef @.str.34)
  %14 = load i32, ptr @lastError, align 4
  %15 = call i32 (ptr, ...) @printf(ptr noundef @.str.35, i32 noundef %14)
  call void @printLogs()
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind }

!llvm.ident = !{!0}
!llvm.module.flags = !{!1, !2, !3}

!0 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
!6 = distinct !{!6, !5}
