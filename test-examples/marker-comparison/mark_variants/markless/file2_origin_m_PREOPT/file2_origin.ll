; ModuleID = './file2_origin_m_PREOPT/file2_origin.bc'
source_filename = "file2_origin.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.__va_list_tag = type { i32, i32, ptr, ptr }

@totalOperations = dso_local local_unnamed_addr global i32 0, align 4
@lastError = dso_local local_unnamed_addr global i32 0, align 4
@logCount = dso_local local_unnamed_addr global i32 0, align 4
@logMessages = dso_local global [100 x [256 x i8]] zeroinitializer, align 16
@.str = private unnamed_addr constant [54 x i8] c"ERROR: Invalid deductResource call (res=%d, amt=%.2f)\00", align 1
@resources = dso_local local_unnamed_addr global [3 x double] zeroinitializer, align 16
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
@.str.29 = private unnamed_addr constant [14 x i8] c"Initial State\00", align 1
@.str.34 = private unnamed_addr constant [12 x i8] c"Final State\00", align 1
@.str.35 = private unnamed_addr constant [22 x i8] c"\0ALast error code: %d\0A\00", align 1
@str = private unnamed_addr constant [23 x i8] c"===== END OF LOG =====\00", align 1
@str.36 = private unnamed_addr constant [60 x i8] c"\0A--- Demo 1: Heavy electricity usage (needs conversion) ---\00", align 1
@str.37 = private unnamed_addr constant [63 x i8] c"\0A--- Demo 2: Produce fuel and convert water to electricity ---\00", align 1
@str.38 = private unnamed_addr constant [83 x i8] c"\0A--- Demo 3: Massive water consumption (beyond reserves and conversion limits) ---\00", align 1
@str.39 = private unnamed_addr constant [50 x i8] c"\0A--- Demo 4: Interleaved production and usage ---\00", align 1

; Function Attrs: nofree nounwind uwtable
define dso_local void @logEvent(ptr noundef readonly captures(none) %0, ...) local_unnamed_addr #0 {
  %2 = alloca [1 x %struct.__va_list_tag], align 16
  %3 = load i32, ptr @logCount, align 4, !tbaa !5
  %4 = icmp sgt i32 %3, 99
  br i1 %4, label %12, label %5

5:                                                ; preds = %1
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %2) #5
  call void @llvm.va_start.p0(ptr nonnull %2)
  %6 = load i32, ptr @logCount, align 4, !tbaa !5
  %7 = sext i32 %6 to i64
  %8 = getelementptr inbounds [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %7
  %9 = call i32 @vsnprintf(ptr noundef nonnull %8, i64 noundef 256, ptr noundef %0, ptr noundef nonnull %2) #5
  call void @llvm.va_end.p0(ptr nonnull %2)
  %10 = load i32, ptr @logCount, align 4, !tbaa !5
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr @logCount, align 4, !tbaa !5
  call void @llvm.lifetime.end.p0(i64 24, ptr nonnull %2) #5
  br label %12

12:                                               ; preds = %5, %1
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #2

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @vsnprintf(ptr noundef captures(none), i64 noundef, ptr noundef readonly captures(none), ptr noundef) local_unnamed_addr #3

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end.p0(ptr) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nofree nounwind uwtable
define dso_local noundef zeroext i1 @deductResource(i32 noundef %0, double noundef %1) local_unnamed_addr #0 {
  %3 = icmp ugt i32 %0, 2
  %4 = fcmp olt double %1, 0.000000e+00
  %5 = or i1 %3, %4
  br i1 %5, label %6, label %7

6:                                                ; preds = %2
  store i32 1, ptr @lastError, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str, i32 noundef %0, double noundef %1)
  br label %25

7:                                                ; preds = %2
  %8 = zext nneg i32 %0 to i64
  %9 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %8
  %10 = load double, ptr %9, align 8, !tbaa !9
  %11 = fcmp ult double %10, %1
  br i1 %11, label %20, label %12

12:                                               ; preds = %7
  %13 = fsub double %10, %1
  store double %13, ptr %9, align 8, !tbaa !9
  %14 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %15 = add nsw i32 %14, 1
  store i32 %15, ptr @totalOperations, align 4, !tbaa !5
  %16 = icmp eq i32 %0, 0
  %17 = icmp eq i32 %0, 1
  %18 = select i1 %17, ptr @.str.3, ptr @.str.4
  %19 = select i1 %16, ptr @.str.2, ptr %18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %1, ptr noundef nonnull %19, double noundef %13)
  br label %25

20:                                               ; preds = %7
  %21 = icmp eq i32 %0, 0
  %22 = icmp eq i32 %0, 1
  %23 = select i1 %22, ptr @.str.3, ptr @.str.4
  %24 = select i1 %21, ptr @.str.2, ptr %23
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.5, ptr noundef nonnull %24, double noundef %10, double noundef %1)
  br label %25

25:                                               ; preds = %20, %12, %6
  %26 = phi i1 [ false, %6 ], [ true, %12 ], [ false, %20 ]
  ret i1 %26
}

; Function Attrs: nofree nounwind uwtable
define dso_local noundef zeroext i1 @addResource(i32 noundef %0, double noundef %1) local_unnamed_addr #0 {
  %3 = icmp ult i32 %0, 3
  %4 = fcmp uge double %1, 0.000000e+00
  %5 = and i1 %3, %4
  br i1 %5, label %7, label %6

6:                                                ; preds = %2
  store i32 2, ptr @lastError, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.6, i32 noundef %0, double noundef %1)
  br label %18

7:                                                ; preds = %2
  %8 = zext nneg i32 %0 to i64
  %9 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %8
  %10 = load double, ptr %9, align 8, !tbaa !9
  %11 = fadd double %1, %10
  store double %11, ptr %9, align 8, !tbaa !9
  %12 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %13 = add nsw i32 %12, 1
  store i32 %13, ptr @totalOperations, align 4, !tbaa !5
  %14 = icmp eq i32 %0, 0
  %15 = icmp eq i32 %0, 1
  %16 = select i1 %15, ptr @.str.3, ptr @.str.4
  %17 = select i1 %14, ptr @.str.2, ptr %16
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %1, ptr noundef nonnull %17, double noundef %11)
  br label %18

18:                                               ; preds = %7, %6
  ret i1 %5
}

; Function Attrs: nofree nounwind uwtable
define dso_local noundef zeroext i1 @convertResourceToGetTarget(i32 noundef %0, i32 noundef %1, double noundef %2, double noundef %3) local_unnamed_addr #0 {
  %5 = icmp eq i32 %0, %1
  br i1 %5, label %6, label %7

6:                                                ; preds = %4
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.8)
  br label %47

7:                                                ; preds = %4
  %8 = fcmp ugt double %2, 0.000000e+00
  br i1 %8, label %9, label %47

9:                                                ; preds = %7
  %10 = fdiv double %2, %3
  %11 = icmp eq i32 %0, 0
  %12 = icmp eq i32 %0, 1
  %13 = select i1 %12, ptr @.str.3, ptr @.str.4
  %14 = select i1 %11, ptr @.str.2, ptr %13
  %15 = icmp eq i32 %1, 0
  %16 = icmp eq i32 %1, 1
  %17 = select i1 %16, ptr @.str.3, ptr @.str.4
  %18 = select i1 %15, ptr @.str.2, ptr %17
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.9, ptr noundef nonnull %14, ptr noundef nonnull %18, double noundef %2, double noundef %10)
  %19 = icmp ugt i32 %0, 2
  %20 = fcmp olt double %10, 0.000000e+00
  %21 = or i1 %19, %20
  br i1 %21, label %22, label %23

22:                                               ; preds = %9
  store i32 1, ptr @lastError, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str, i32 noundef %0, double noundef %10)
  br label %46

23:                                               ; preds = %9
  %24 = zext nneg i32 %0 to i64
  %25 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %24
  %26 = load double, ptr %25, align 8, !tbaa !9
  %27 = fcmp ult double %26, %10
  br i1 %27, label %28, label %29

28:                                               ; preds = %23
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.5, ptr noundef nonnull %14, double noundef %26, double noundef %10)
  br label %46

29:                                               ; preds = %23
  %30 = fsub double %26, %10
  store double %30, ptr %25, align 8, !tbaa !9
  %31 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %32 = add nsw i32 %31, 1
  store i32 %32, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %10, ptr noundef nonnull %14, double noundef %30)
  %33 = icmp ult i32 %1, 3
  br i1 %33, label %34, label %41

34:                                               ; preds = %29
  %35 = zext nneg i32 %1 to i64
  %36 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %35
  %37 = load double, ptr %36, align 8, !tbaa !9
  %38 = fadd double %2, %37
  store double %38, ptr %36, align 8, !tbaa !9
  %39 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %40 = add nsw i32 %39, 1
  store i32 %40, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %2, ptr noundef nonnull %18, double noundef %38)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.10, double noundef %10, double noundef %2)
  br label %47

41:                                               ; preds = %29
  store i32 2, ptr @lastError, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.6, i32 noundef %1, double noundef %2)
  %42 = load double, ptr %25, align 8, !tbaa !9
  %43 = fadd double %10, %42
  store double %43, ptr %25, align 8, !tbaa !9
  %44 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %45 = add nsw i32 %44, 1
  store i32 %45, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %10, ptr noundef nonnull %14, double noundef %43)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.11)
  br label %47

46:                                               ; preds = %28, %22
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.12, ptr noundef nonnull %14)
  br label %47

47:                                               ; preds = %46, %41, %34, %7, %6
  %48 = phi i1 [ false, %6 ], [ true, %7 ], [ true, %34 ], [ false, %41 ], [ false, %46 ]
  ret i1 %48
}

; Function Attrs: nofree nounwind uwtable
define dso_local noundef zeroext i1 @obtainResource(i32 noundef %0, double noundef %1) local_unnamed_addr #0 {
  %3 = fcmp ugt double %1, 0.000000e+00
  br i1 %3, label %4, label %23

4:                                                ; preds = %2
  %5 = icmp eq i32 %0, 0
  %6 = icmp eq i32 %0, 1
  %7 = select i1 %6, ptr @.str.3, ptr @.str.4
  %8 = select i1 %5, ptr @.str.2, ptr %7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.13, double noundef %1, ptr noundef nonnull %8)
  br i1 %5, label %14, label %9

9:                                                ; preds = %4
  %10 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %10, label %21, label %11

11:                                               ; preds = %9
  br i1 %6, label %18, label %12

12:                                               ; preds = %11
  %13 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 1, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %13, label %21, label %16

14:                                               ; preds = %4
  %15 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 1, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %15, label %21, label %18

16:                                               ; preds = %12
  %17 = icmp eq i32 %0, 2
  br i1 %17, label %20, label %18

18:                                               ; preds = %16, %14, %11
  %19 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 2, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %19, label %21, label %20

20:                                               ; preds = %18, %16
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.15, double noundef %1, ptr noundef nonnull %8)
  br label %23

21:                                               ; preds = %18, %14, %12, %9
  %22 = phi ptr [ @.str.2, %9 ], [ @.str.3, %12 ], [ @.str.4, %18 ], [ @.str.3, %14 ]
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.14, ptr noundef nonnull %22)
  br label %23

23:                                               ; preds = %21, %20, %2
  %24 = phi i1 [ false, %20 ], [ true, %21 ], [ true, %2 ]
  ret i1 %24
}

; Function Attrs: nofree nounwind uwtable
define dso_local noundef zeroext i1 @useResource(i32 noundef %0, double noundef %1) local_unnamed_addr #0 {
  %3 = fcmp ugt double %1, 0.000000e+00
  br i1 %3, label %4, label %39

4:                                                ; preds = %2
  %5 = icmp eq i32 %0, 0
  %6 = icmp eq i32 %0, 1
  %7 = select i1 %6, ptr @.str.3, ptr @.str.4
  %8 = select i1 %5, ptr @.str.2, ptr %7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.16, double noundef %1, ptr noundef nonnull %8)
  %9 = sext i32 %0 to i64
  %10 = getelementptr inbounds [3 x double], ptr @resources, i64 0, i64 %9
  %11 = load double, ptr %10, align 8, !tbaa !9
  %12 = fcmp ult double %11, %1
  br i1 %12, label %22, label %13

13:                                               ; preds = %4
  %14 = icmp ugt i32 %0, 2
  br i1 %14, label %15, label %16

15:                                               ; preds = %13
  store i32 1, ptr @lastError, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str, i32 noundef %0, double noundef %1)
  br label %39

16:                                               ; preds = %13
  %17 = zext nneg i32 %0 to i64
  %18 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %17
  %19 = fsub double %11, %1
  store double %19, ptr %18, align 8, !tbaa !9
  %20 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %21 = add nsw i32 %20, 1
  store i32 %21, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %1, ptr noundef nonnull %8, double noundef %19)
  br label %39

22:                                               ; preds = %4
  %23 = fsub double %1, %11
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.17, double noundef %23)
  %24 = tail call zeroext i1 @obtainResource(i32 noundef %0, double noundef %23)
  br i1 %24, label %25, label %38

25:                                               ; preds = %22
  %26 = icmp ugt i32 %0, 2
  br i1 %26, label %27, label %28

27:                                               ; preds = %25
  store i32 1, ptr @lastError, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str, i32 noundef %0, double noundef %1)
  br label %39

28:                                               ; preds = %25
  %29 = zext nneg i32 %0 to i64
  %30 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %29
  %31 = load double, ptr %30, align 8, !tbaa !9
  %32 = fcmp ult double %31, %1
  br i1 %32, label %37, label %33

33:                                               ; preds = %28
  %34 = fsub double %31, %1
  store double %34, ptr %30, align 8, !tbaa !9
  %35 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %36 = add nsw i32 %35, 1
  store i32 %36, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %1, ptr noundef nonnull %8, double noundef %34)
  br label %39

37:                                               ; preds = %28
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.5, ptr noundef nonnull %8, double noundef %31, double noundef %1)
  br label %39

38:                                               ; preds = %22
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.18, double noundef %1, ptr noundef nonnull %8)
  store i32 3, ptr @lastError, align 4, !tbaa !5
  br label %39

39:                                               ; preds = %38, %37, %33, %27, %16, %15, %2
  %40 = phi i1 [ true, %2 ], [ false, %38 ], [ false, %15 ], [ true, %16 ], [ false, %27 ], [ true, %33 ], [ false, %37 ]
  ret i1 %40
}

; Function Attrs: nofree nounwind uwtable
define dso_local noundef zeroext i1 @produceResource(i32 noundef %0, double noundef %1) local_unnamed_addr #0 {
  %3 = fcmp olt double %1, 0.000000e+00
  br i1 %3, label %23, label %4

4:                                                ; preds = %2
  %5 = icmp eq i32 %0, 0
  br i1 %5, label %6, label %11

6:                                                ; preds = %4
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef %1, ptr noundef nonnull @.str.2)
  %7 = load double, ptr @resources, align 16, !tbaa !9
  %8 = fadd double %1, %7
  store double %8, ptr @resources, align 16, !tbaa !9
  %9 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %10 = add nsw i32 %9, 1
  store i32 %10, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %1, ptr noundef nonnull @.str.2, double noundef %8)
  br label %23

11:                                               ; preds = %4
  %12 = icmp eq i32 %0, 1
  %13 = select i1 %12, ptr @.str.3, ptr @.str.4
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef %1, ptr noundef nonnull %13)
  %14 = icmp ult i32 %0, 3
  br i1 %14, label %16, label %15

15:                                               ; preds = %11
  store i32 2, ptr @lastError, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.6, i32 noundef %0, double noundef %1)
  br label %23

16:                                               ; preds = %11
  %17 = zext nneg i32 %0 to i64
  %18 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %17
  %19 = load double, ptr %18, align 8, !tbaa !9
  %20 = fadd double %1, %19
  store double %20, ptr %18, align 8, !tbaa !9
  %21 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %22 = add nsw i32 %21, 1
  store i32 %22, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %1, ptr noundef nonnull %13, double noundef %20)
  br label %23

23:                                               ; preds = %16, %15, %6, %2
  %24 = phi i1 [ false, %2 ], [ true, %6 ], [ false, %15 ], [ true, %16 ]
  ret i1 %24
}

; Function Attrs: nofree nounwind uwtable
define dso_local void @printResources(ptr noundef %0) local_unnamed_addr #0 {
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.20, ptr noundef %0)
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.21, ptr noundef %0)
  %3 = load double, ptr @resources, align 16, !tbaa !9
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.22, double noundef %3)
  %5 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !9
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.23, double noundef %5)
  %7 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.24, double noundef %7)
  %9 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %10 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.25, i32 noundef %9)
  ret void
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

; Function Attrs: nofree nounwind uwtable
define dso_local void @printLogs() local_unnamed_addr #0 {
  %1 = load i32, ptr @logCount, align 4, !tbaa !5
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.26, i32 noundef %1)
  %3 = load i32, ptr @logCount, align 4, !tbaa !5
  %4 = icmp sgt i32 %3, 0
  br i1 %4, label %7, label %5

5:                                                ; preds = %7, %0
  %6 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  ret void

7:                                                ; preds = %7, %0
  %8 = phi i64 [ %11, %7 ], [ 0, %0 ]
  %9 = getelementptr inbounds nuw [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %8
  %10 = tail call i32 @puts(ptr nonnull dereferenceable(1) %9)
  %11 = add nuw nsw i64 %8, 1
  %12 = load i32, ptr @logCount, align 4, !tbaa !5
  %13 = sext i32 %12 to i64
  %14 = icmp slt i64 %11, %13
  br i1 %14, label %7, label %5, !llvm.loop !11
}

; Function Attrs: nofree nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #0 {
  store <2 x double> <double 1.000000e+02, double 5.000000e+01>, ptr @resources, align 16, !tbaa !9
  store double 8.000000e+01, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  store i32 0, ptr @totalOperations, align 4, !tbaa !5
  store i32 0, ptr @lastError, align 4, !tbaa !5
  store i32 0, ptr @logCount, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.20, ptr noundef nonnull @.str.29)
  %1 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.21, ptr noundef nonnull @.str.29)
  %2 = load double, ptr @resources, align 16, !tbaa !9
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.22, double noundef %2)
  %4 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !9
  %5 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.23, double noundef %4)
  %6 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.24, double noundef %6)
  %8 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %9 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.25, i32 noundef %8)
  %10 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.36)
  %11 = tail call zeroext i1 @useResource(i32 noundef 1, double noundef 7.000000e+01)
  %12 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.37)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 3.000000e+01, ptr noundef nonnull @.str.4)
  %13 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  %14 = fadd double %13, 3.000000e+01
  store double %14, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  %15 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %16 = add nsw i32 %15, 1
  store i32 %16, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 3.000000e+01, ptr noundef nonnull @.str.4, double noundef %14)
  %17 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef 1, double noundef 1.500000e+01, double noundef 8.000000e-01)
  %18 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.38)
  %19 = tail call zeroext i1 @useResource(i32 noundef 0, double noundef 1.200000e+02)
  %20 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.39)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 1.000000e+01, ptr noundef nonnull @.str.3)
  %21 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !9
  %22 = fadd double %21, 1.000000e+01
  store double %22, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !9
  %23 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %24 = add nsw i32 %23, 1
  store i32 %24, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 1.000000e+01, ptr noundef nonnull @.str.3, double noundef %22)
  %25 = tail call zeroext i1 @useResource(i32 noundef 2, double noundef 5.000000e+01)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 4.000000e+01, ptr noundef nonnull @.str.2)
  %26 = load double, ptr @resources, align 16, !tbaa !9
  %27 = fadd double %26, 4.000000e+01
  store double %27, ptr @resources, align 16, !tbaa !9
  %28 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %29 = add nsw i32 %28, 1
  store i32 %29, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 4.000000e+01, ptr noundef nonnull @.str.2, double noundef %27)
  %30 = tail call zeroext i1 @useResource(i32 noundef 1, double noundef 2.500000e+01)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.20, ptr noundef nonnull @.str.34)
  %31 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.21, ptr noundef nonnull @.str.34)
  %32 = load double, ptr @resources, align 16, !tbaa !9
  %33 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.22, double noundef %32)
  %34 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !9
  %35 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.23, double noundef %34)
  %36 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  %37 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.24, double noundef %36)
  %38 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %39 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.25, i32 noundef %38)
  %40 = load i32, ptr @lastError, align 4, !tbaa !5
  %41 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.35, i32 noundef %40)
  %42 = load i32, ptr @logCount, align 4, !tbaa !5
  %43 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.26, i32 noundef %42)
  %44 = load i32, ptr @logCount, align 4, !tbaa !5
  %45 = icmp sgt i32 %44, 0
  br i1 %45, label %46, label %54

46:                                               ; preds = %46, %0
  %47 = phi i64 [ %50, %46 ], [ 0, %0 ]
  %48 = getelementptr inbounds nuw [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %47
  %49 = tail call i32 @puts(ptr nonnull dereferenceable(1) %48)
  %50 = add nuw nsw i64 %47, 1
  %51 = load i32, ptr @logCount, align 4, !tbaa !5
  %52 = sext i32 %51 to i64
  %53 = icmp slt i64 %50, %52
  br i1 %53, label %46, label %54, !llvm.loop !11

54:                                               ; preds = %46, %0
  %55 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #4

attributes #0 = { nofree nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nocallback nofree nosync nounwind willreturn }
attributes #3 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree nounwind }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 1, !"ThinLTO", i32 0}
!3 = !{i32 1, !"EnableSplitLTOUnit", i32 1}
!4 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = !{!10, !10, i64 0}
!10 = !{!"double", !7, i64 0}
!11 = distinct !{!11, !12}
!12 = !{!"llvm.loop.mustprogress"}

^0 = module: (path: "./file2_origin_m_PREOPT/file2_origin.bc", hash: (0, 0, 0, 0, 0))
^1 = gv: (name: ".str", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 2532754695280066
^2 = gv: (name: ".str.19", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 375696106327838781
^3 = gv: (name: ".str.16", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 781007208800804852
^4 = gv: (name: "llvm.va_start.p0") ; guid = 1116014281982105379
^5 = gv: (name: "str", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 1883988536932498948
^6 = gv: (name: ".str.35", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 2669808366760954928
^7 = gv: (name: "str.38", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 2678899511415816208
^8 = gv: (name: "str.36", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 2964658042934851143
^9 = gv: (name: ".str.8", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 3187368262959527971
^10 = gv: (name: ".str.25", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 3327144135029162869
^11 = gv: (name: ".str.26", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 3450655244342370486
^12 = gv: (name: ".str.24", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 3694601586416641942
^13 = gv: (name: "logCount", summaries: (variable: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 1, constant: 0)))) ; guid = 4035969925966406128
^14 = gv: (name: "obtainResource", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 26, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^55, tail: 1), (callee: ^36, tail: 1)), refs: (^52, ^38, ^45, ^41, ^34, ^32)))) ; guid = 4127045675581085826
^15 = gv: (name: "llvm.lifetime.start.p0") ; guid = 4736283418132453162
^16 = gv: (name: "lastError", summaries: (variable: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 1, constant: 0)))) ; guid = 4808874296988906674
^17 = gv: (name: ".str.5", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 5028106286570996766
^18 = gv: (name: "llvm.va_end.p0") ; guid = 5090975557526790353
^19 = gv: (name: "deductResource", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 31, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^55, tail: 1)), refs: (^16, ^1, ^26, ^33, ^52, ^38, ^45, ^46, ^17)))) ; guid = 5195368790120632634
^20 = gv: (name: ".str.7", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 5631185492959052836
^21 = gv: (name: "logMessages", summaries: (variable: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 1, constant: 0)))) ; guid = 5693580606693062806
^22 = gv: (name: ".str.22", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 6402080721108053552
^23 = gv: (name: ".str.6", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 6887403461103636191
^24 = gv: (name: ".str.18", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 7145633163540218612
^25 = gv: (name: "printf") ; guid = 7383291119112528047
^26 = gv: (name: "resources", summaries: (variable: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 1, constant: 0)))) ; guid = 7930419960199296341
^27 = gv: (name: ".str.11", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 7942196076101584126
^28 = gv: (name: "vsnprintf") ; guid = 8260034984128293968
^29 = gv: (name: ".str.21", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 8825247399407373734
^30 = gv: (name: "puts") ; guid = 8979701042202144121
^31 = gv: (name: "printLogs", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 15, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^25, tail: 1), (callee: ^30, tail: 1)), refs: (^13, ^11, ^5, ^21)))) ; guid = 9999713285701850125
^32 = gv: (name: ".str.14", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 10230019618414349501
^33 = gv: (name: "totalOperations", summaries: (variable: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 1, constant: 0)))) ; guid = 10287797576432768888
^34 = gv: (name: ".str.15", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 10695624196320927352
^35 = gv: (name: "str.37", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 12056547863414187638
^36 = gv: (name: "convertResourceToGetTarget", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 64, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^55, tail: 1)), refs: (^9, ^52, ^38, ^45, ^50, ^16, ^1, ^26, ^17, ^33, ^46, ^20, ^43, ^23, ^27, ^57)))) ; guid = 12116041630464161402
^37 = gv: (name: ".str.17", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 12691897982921208210
^38 = gv: (name: ".str.4", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 13025585774940304990
^39 = gv: (name: "llvm.lifetime.end.p0") ; guid = 13244677477227836793
^40 = gv: (name: ".str.20", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 13392787100258444056
^41 = gv: (name: ".str.13", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 14434237082905948652
^42 = gv: (name: "useResource", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 54, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^55, tail: 1), (callee: ^14, tail: 1)), refs: (^52, ^38, ^45, ^3, ^26, ^16, ^1, ^33, ^46, ^37, ^17, ^24)))) ; guid = 15323274007057276802
^43 = gv: (name: ".str.10", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 15348317746444497813
^44 = gv: (name: ".str.29", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 15493424912265585829
^45 = gv: (name: ".str.2", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 15748144372332436015
^46 = gv: (name: ".str.1", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 15757972892107832524
^47 = gv: (name: "main", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 75, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^55, tail: 1), (callee: ^25, tail: 1), (callee: ^30, tail: 1), (callee: ^42, tail: 1), (callee: ^36, tail: 1)), refs: (^26, ^33, ^16, ^13, ^40, ^44, ^29, ^22, ^53, ^12, ^10, ^8, ^35, ^2, ^38, ^20, ^7, ^51, ^52, ^45, ^49, ^6, ^11, ^21, ^5)))) ; guid = 15822663052811949562
^48 = gv: (name: "produceResource", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 33, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^55, tail: 1)), refs: (^2, ^45, ^26, ^33, ^20, ^52, ^38, ^16, ^23)))) ; guid = 16477953438343050136
^49 = gv: (name: ".str.34", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 16482933451644558437
^50 = gv: (name: ".str.9", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 16645872452634559675
^51 = gv: (name: "str.39", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 16852225307648081945
^52 = gv: (name: ".str.3", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 17508981887875708411
^53 = gv: (name: ".str.23", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 17592669117849567373
^54 = gv: (name: "addResource", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 22, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^55, tail: 1)), refs: (^16, ^23, ^26, ^33, ^52, ^38, ^45, ^20)))) ; guid = 17593996893088219275
^55 = gv: (name: "logEvent", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 17, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^28)), refs: (^13, ^21)))) ; guid = 18059553383448241467
^56 = gv: (name: "printResources", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 11, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^55, tail: 1), (callee: ^25, tail: 1)), refs: (^40, ^29, ^26, ^22, ^53, ^12, ^33, ^10)))) ; guid = 18270205028877567917
^57 = gv: (name: ".str.12", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 18378179541384066302
^58 = flags: 8
^59 = blockcount: 0
