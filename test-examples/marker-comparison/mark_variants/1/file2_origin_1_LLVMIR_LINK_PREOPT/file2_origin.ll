; ModuleID = './file2_origin_1_LLVMIR_LINK_PREOPT/file2_origin.bc'
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
@llvm.compiler.used = appending global [1 x ptr] [ptr @codemarker_alpha], section "llvm.metadata"

; Function Attrs: noinline nounwind uwtable
define internal void @codemarker_alpha(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  call void @llvm.lifetime.start.p0(i64 4, ptr nonnull %2)
  store volatile i32 %0, ptr %2, align 4, !tbaa !5
  tail call void asm sideeffect "", "r,~{memory},~{dirflag},~{fpsr},~{flags}"(i32 %0) #6, !srcloc !9
  call void @llvm.lifetime.end.p0(i64 4, ptr nonnull %2)
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nounwind uwtable
define dso_local void @logEvent(ptr noundef readonly captures(none) %0, ...) local_unnamed_addr #2 {
  %2 = alloca [1 x %struct.__va_list_tag], align 16
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !10
  tail call void @codemarker_alpha(i32 noundef 0) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !11
  %3 = load i32, ptr @logCount, align 4, !tbaa !5
  %4 = icmp sgt i32 %3, 99
  br i1 %4, label %12, label %5

5:                                                ; preds = %1
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %2) #6
  call void @llvm.va_start.p0(ptr nonnull %2)
  %6 = load i32, ptr @logCount, align 4, !tbaa !5
  %7 = sext i32 %6 to i64
  %8 = getelementptr inbounds [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %7
  %9 = call i32 @vsnprintf(ptr noundef nonnull %8, i64 noundef 256, ptr noundef %0, ptr noundef nonnull %2) #6
  call void @llvm.va_end.p0(ptr nonnull %2)
  %10 = load i32, ptr @logCount, align 4, !tbaa !5
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr @logCount, align 4, !tbaa !5
  call void @llvm.lifetime.end.p0(i64 24, ptr nonnull %2) #6
  br label %12

12:                                               ; preds = %5, %1
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #3

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @vsnprintf(ptr noundef captures(none), i64 noundef, ptr noundef readonly captures(none), ptr noundef) local_unnamed_addr #4

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end.p0(ptr) #3

; Function Attrs: nounwind uwtable
define dso_local noundef zeroext i1 @deductResource(i32 noundef %0, double noundef %1) local_unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !12
  tail call void @codemarker_alpha(i32 noundef 1) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !13
  %3 = icmp ugt i32 %0, 2
  %4 = fcmp olt double %1, 0.000000e+00
  %5 = or i1 %3, %4
  br i1 %5, label %6, label %7

6:                                                ; preds = %2
  store i32 1, ptr @lastError, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !14
  tail call void @codemarker_alpha(i32 noundef 2) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !15
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str, i32 noundef %0, double noundef %1)
  br label %27

7:                                                ; preds = %2
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !16
  tail call void @codemarker_alpha(i32 noundef 3) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !17
  %8 = zext nneg i32 %0 to i64
  %9 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %8
  %10 = load double, ptr %9, align 8, !tbaa !18
  %11 = fcmp ult double %10, %1
  br i1 %11, label %21, label %12

12:                                               ; preds = %7
  %13 = fsub double %10, %1
  store double %13, ptr %9, align 8, !tbaa !18
  %14 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %15 = add nsw i32 %14, 1
  store i32 %15, ptr @totalOperations, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !20
  tail call void @codemarker_alpha(i32 noundef 4) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !21
  %16 = icmp eq i32 %0, 0
  %17 = icmp eq i32 %0, 1
  %18 = select i1 %17, ptr @.str.3, ptr @.str.4
  %19 = select i1 %16, ptr @.str.2, ptr %18
  %20 = load double, ptr %9, align 8, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %1, ptr noundef nonnull %19, double noundef %20)
  br label %27

21:                                               ; preds = %7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !22
  tail call void @codemarker_alpha(i32 noundef 5) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !23
  %22 = icmp eq i32 %0, 0
  %23 = icmp eq i32 %0, 1
  %24 = select i1 %23, ptr @.str.3, ptr @.str.4
  %25 = select i1 %22, ptr @.str.2, ptr %24
  %26 = load double, ptr %9, align 8, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.5, ptr noundef nonnull %25, double noundef %26, double noundef %1)
  br label %27

27:                                               ; preds = %21, %12, %6
  %28 = phi i1 [ false, %6 ], [ true, %12 ], [ false, %21 ]
  ret i1 %28
}

; Function Attrs: nounwind uwtable
define dso_local noundef zeroext i1 @addResource(i32 noundef %0, double noundef %1) local_unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !24
  tail call void @codemarker_alpha(i32 noundef 6) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !25
  %3 = icmp ult i32 %0, 3
  %4 = fcmp uge double %1, 0.000000e+00
  %5 = and i1 %3, %4
  br i1 %5, label %7, label %6

6:                                                ; preds = %2
  store i32 2, ptr @lastError, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !26
  tail call void @codemarker_alpha(i32 noundef 7) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !27
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.6, i32 noundef %0, double noundef %1)
  br label %19

7:                                                ; preds = %2
  %8 = zext nneg i32 %0 to i64
  %9 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %8
  %10 = load double, ptr %9, align 8, !tbaa !18
  %11 = fadd double %1, %10
  store double %11, ptr %9, align 8, !tbaa !18
  %12 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %13 = add nsw i32 %12, 1
  store i32 %13, ptr @totalOperations, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !28
  tail call void @codemarker_alpha(i32 noundef 8) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !29
  %14 = icmp eq i32 %0, 0
  %15 = icmp eq i32 %0, 1
  %16 = select i1 %15, ptr @.str.3, ptr @.str.4
  %17 = select i1 %14, ptr @.str.2, ptr %16
  %18 = load double, ptr %9, align 8, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %1, ptr noundef nonnull %17, double noundef %18)
  br label %19

19:                                               ; preds = %7, %6
  ret i1 %5
}

; Function Attrs: nounwind uwtable
define dso_local noundef zeroext i1 @convertResourceToGetTarget(i32 noundef %0, i32 noundef %1, double noundef %2, double noundef %3) local_unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !30
  tail call void @codemarker_alpha(i32 noundef 9) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !31
  %5 = icmp eq i32 %0, %1
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6
  br i1 %5, label %6, label %7

6:                                                ; preds = %4
  tail call void @codemarker_alpha(i32 noundef 10) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !32
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.8)
  br label %26

7:                                                ; preds = %4
  tail call void @codemarker_alpha(i32 noundef 11) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !33
  %8 = fcmp ugt double %2, 0.000000e+00
  br i1 %8, label %9, label %26

9:                                                ; preds = %7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !34
  tail call void @codemarker_alpha(i32 noundef 12) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !35
  %10 = fdiv double %2, %3
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !36
  tail call void @codemarker_alpha(i32 noundef 13) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !37
  %11 = icmp eq i32 %0, 0
  %12 = icmp eq i32 %0, 1
  %13 = select i1 %12, ptr @.str.3, ptr @.str.4
  %14 = select i1 %11, ptr @.str.2, ptr %13
  %15 = icmp eq i32 %1, 0
  %16 = icmp eq i32 %1, 1
  %17 = select i1 %16, ptr @.str.3, ptr @.str.4
  %18 = select i1 %15, ptr @.str.2, ptr %17
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.9, ptr noundef nonnull %14, ptr noundef nonnull %18, double noundef %2, double noundef %10)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !38
  tail call void @codemarker_alpha(i32 noundef 14) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !39
  %19 = tail call zeroext i1 @deductResource(i32 noundef %0, double noundef %10)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6
  br i1 %19, label %20, label %25

20:                                               ; preds = %9
  tail call void @codemarker_alpha(i32 noundef 15) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !40
  %21 = tail call zeroext i1 @addResource(i32 noundef %1, double noundef %2)
  br i1 %21, label %22, label %23

22:                                               ; preds = %20
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !41
  tail call void @codemarker_alpha(i32 noundef 16) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !42
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.10, double noundef %10, double noundef %2)
  br label %26

23:                                               ; preds = %20
  %24 = tail call zeroext i1 @addResource(i32 noundef %0, double noundef %10)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !43
  tail call void @codemarker_alpha(i32 noundef 17) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !44
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.11)
  br label %26

25:                                               ; preds = %9
  tail call void @codemarker_alpha(i32 noundef 18) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !45
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.12, ptr noundef nonnull %14)
  br label %26

26:                                               ; preds = %25, %23, %22, %7, %6
  %27 = phi i1 [ false, %6 ], [ true, %7 ], [ true, %22 ], [ false, %23 ], [ false, %25 ]
  ret i1 %27
}

; Function Attrs: nounwind uwtable
define dso_local noundef zeroext i1 @obtainResource(i32 noundef %0, double noundef %1) local_unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !46
  tail call void @codemarker_alpha(i32 noundef 19) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !47
  %3 = fcmp ugt double %1, 0.000000e+00
  br i1 %3, label %4, label %21

4:                                                ; preds = %2
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !48
  tail call void @codemarker_alpha(i32 noundef 20) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !49
  %5 = icmp eq i32 %0, 0
  %6 = icmp eq i32 %0, 1
  %7 = select i1 %6, ptr @.str.3, ptr @.str.4
  %8 = select i1 %5, ptr @.str.2, ptr %7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.13, double noundef %1, ptr noundef nonnull %8)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !50
  tail call void @codemarker_alpha(i32 noundef 21) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !51
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !52
  tail call void @codemarker_alpha(i32 noundef 22) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !53
  br i1 %5, label %11, label %9

9:                                                ; preds = %4
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !54
  tail call void @codemarker_alpha(i32 noundef 23) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !55
  %10 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %10, label %19, label %11

11:                                               ; preds = %9, %4
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !52
  tail call void @codemarker_alpha(i32 noundef 22) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !53
  br i1 %6, label %14, label %12

12:                                               ; preds = %11
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !54
  tail call void @codemarker_alpha(i32 noundef 23) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !55
  %13 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 1, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %13, label %19, label %14

14:                                               ; preds = %12, %11
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !52
  tail call void @codemarker_alpha(i32 noundef 22) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !53
  %15 = icmp eq i32 %0, 2
  br i1 %15, label %18, label %16

16:                                               ; preds = %14
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !54
  tail call void @codemarker_alpha(i32 noundef 23) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !55
  %17 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 2, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %17, label %19, label %18

18:                                               ; preds = %16, %14
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !56
  tail call void @codemarker_alpha(i32 noundef 25) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !57
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.15, double noundef %1, ptr noundef nonnull %8)
  br label %21

19:                                               ; preds = %16, %12, %9
  %20 = phi ptr [ @.str.2, %9 ], [ @.str.3, %12 ], [ @.str.4, %16 ]
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !58
  tail call void @codemarker_alpha(i32 noundef 24) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !59
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.14, ptr noundef nonnull %20)
  br label %21

21:                                               ; preds = %19, %18, %2
  %22 = phi i1 [ false, %18 ], [ true, %19 ], [ true, %2 ]
  ret i1 %22
}

; Function Attrs: nounwind uwtable
define dso_local noundef zeroext i1 @useResource(i32 noundef %0, double noundef %1) local_unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !60
  tail call void @codemarker_alpha(i32 noundef 26) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !61
  %3 = fcmp ugt double %1, 0.000000e+00
  br i1 %3, label %4, label %22

4:                                                ; preds = %2
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !62
  tail call void @codemarker_alpha(i32 noundef 27) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !63
  %5 = icmp eq i32 %0, 0
  %6 = icmp eq i32 %0, 1
  %7 = select i1 %6, ptr @.str.3, ptr @.str.4
  %8 = select i1 %5, ptr @.str.2, ptr %7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.16, double noundef %1, ptr noundef nonnull %8)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !64
  tail call void @codemarker_alpha(i32 noundef 28) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !65
  %9 = sext i32 %0 to i64
  %10 = getelementptr inbounds [3 x double], ptr @resources, i64 0, i64 %9
  %11 = load double, ptr %10, align 8, !tbaa !18
  %12 = fcmp ult double %11, %1
  br i1 %12, label %15, label %13

13:                                               ; preds = %4
  %14 = tail call zeroext i1 @deductResource(i32 noundef %0, double noundef %1)
  br label %22

15:                                               ; preds = %4
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !66
  tail call void @codemarker_alpha(i32 noundef 29) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !67
  %16 = load double, ptr %10, align 8, !tbaa !18
  %17 = fsub double %1, %16
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !68
  tail call void @codemarker_alpha(i32 noundef 30) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !69
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.17, double noundef %17)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !70
  tail call void @codemarker_alpha(i32 noundef 31) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !71
  %18 = tail call zeroext i1 @obtainResource(i32 noundef %0, double noundef %17)
  br i1 %18, label %19, label %21

19:                                               ; preds = %15
  %20 = tail call zeroext i1 @deductResource(i32 noundef %0, double noundef %1)
  br label %22

21:                                               ; preds = %15
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !72
  tail call void @codemarker_alpha(i32 noundef 32) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !73
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.18, double noundef %1, ptr noundef nonnull %8)
  store i32 3, ptr @lastError, align 4, !tbaa !5
  br label %22

22:                                               ; preds = %21, %19, %13, %2
  %23 = phi i1 [ %14, %13 ], [ true, %2 ], [ %20, %19 ], [ false, %21 ]
  ret i1 %23
}

; Function Attrs: nounwind uwtable
define dso_local noundef zeroext i1 @produceResource(i32 noundef %0, double noundef %1) local_unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !74
  tail call void @codemarker_alpha(i32 noundef 33) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !75
  %3 = fcmp olt double %1, 0.000000e+00
  br i1 %3, label %16, label %4

4:                                                ; preds = %2
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !76
  tail call void @codemarker_alpha(i32 noundef 34) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !77
  %5 = icmp eq i32 %0, 0
  br i1 %5, label %6, label %12

6:                                                ; preds = %4
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef %1, ptr noundef nonnull @.str.2)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !24
  tail call void @codemarker_alpha(i32 noundef 6) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !25
  %7 = load double, ptr @resources, align 16, !tbaa !18
  %8 = fadd double %1, %7
  store double %8, ptr @resources, align 16, !tbaa !18
  %9 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %10 = add nsw i32 %9, 1
  store i32 %10, ptr @totalOperations, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !28
  tail call void @codemarker_alpha(i32 noundef 8) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !29
  %11 = load double, ptr @resources, align 16, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %1, ptr noundef nonnull @.str.2, double noundef %11)
  br label %16

12:                                               ; preds = %4
  %13 = icmp eq i32 %0, 1
  %14 = select i1 %13, ptr @.str.3, ptr @.str.4
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef %1, ptr noundef nonnull %14)
  %15 = tail call zeroext i1 @addResource(i32 noundef %0, double noundef %1)
  br label %16

16:                                               ; preds = %12, %6, %2
  %17 = phi i1 [ false, %2 ], [ %15, %12 ], [ true, %6 ]
  ret i1 %17
}

; Function Attrs: nounwind uwtable
define dso_local void @printResources(ptr noundef %0) local_unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !78
  tail call void @codemarker_alpha(i32 noundef 35) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !79
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.20, ptr noundef %0)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !80
  tail call void @codemarker_alpha(i32 noundef 36) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !81
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.21, ptr noundef %0)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !82
  tail call void @codemarker_alpha(i32 noundef 37) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !83
  %3 = load double, ptr @resources, align 16, !tbaa !18
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.22, double noundef %3)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !84
  tail call void @codemarker_alpha(i32 noundef 38) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !85
  %5 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !18
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.23, double noundef %5)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !86
  tail call void @codemarker_alpha(i32 noundef 39) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !87
  %7 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !18
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.24, double noundef %7)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !88
  tail call void @codemarker_alpha(i32 noundef 40) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !89
  %9 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %10 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.25, i32 noundef %9)
  ret void
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #4

; Function Attrs: nounwind uwtable
define dso_local void @printLogs() local_unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !90
  tail call void @codemarker_alpha(i32 noundef 41) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !91
  %1 = load i32, ptr @logCount, align 4, !tbaa !5
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.26, i32 noundef %1)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !92
  tail call void @codemarker_alpha(i32 noundef 42) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !93
  %3 = load i32, ptr @logCount, align 4, !tbaa !5
  %4 = icmp sgt i32 %3, 0
  br i1 %4, label %7, label %5

5:                                                ; preds = %7, %0
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !94
  tail call void @codemarker_alpha(i32 noundef 44) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !95
  %6 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  ret void

7:                                                ; preds = %7, %0
  %8 = phi i64 [ %11, %7 ], [ 0, %0 ]
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !96
  tail call void @codemarker_alpha(i32 noundef 43) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !97
  %9 = getelementptr inbounds nuw [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %8
  %10 = tail call i32 @puts(ptr nonnull dereferenceable(1) %9)
  %11 = add nuw nsw i64 %8, 1
  %12 = load i32, ptr @logCount, align 4, !tbaa !5
  %13 = sext i32 %12 to i64
  %14 = icmp slt i64 %11, %13
  br i1 %14, label %7, label %5, !llvm.loop !98
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #2 {
  store <2 x double> <double 1.000000e+02, double 5.000000e+01>, ptr @resources, align 16, !tbaa !18
  store double 8.000000e+01, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !18
  store i32 0, ptr @totalOperations, align 4, !tbaa !5
  store i32 0, ptr @lastError, align 4, !tbaa !5
  store i32 0, ptr @logCount, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !100
  tail call void @codemarker_alpha(i32 noundef 45) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !101
  tail call void @printResources(ptr noundef nonnull @.str.29)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !102
  tail call void @codemarker_alpha(i32 noundef 46) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !103
  %1 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.36)
  %2 = tail call zeroext i1 @useResource(i32 noundef 1, double noundef 7.000000e+01)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !104
  tail call void @codemarker_alpha(i32 noundef 47) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !105
  %3 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.37)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !74
  tail call void @codemarker_alpha(i32 noundef 33) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !75
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !76
  tail call void @codemarker_alpha(i32 noundef 34) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !77
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 3.000000e+01, ptr noundef nonnull @.str.4)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !24
  tail call void @codemarker_alpha(i32 noundef 6) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !25
  %4 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !18
  %5 = fadd double %4, 3.000000e+01
  store double %5, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !18
  %6 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %7 = add nsw i32 %6, 1
  store i32 %7, ptr @totalOperations, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !28
  tail call void @codemarker_alpha(i32 noundef 8) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !29
  %8 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 3.000000e+01, ptr noundef nonnull @.str.4, double noundef %8)
  %9 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef 1, double noundef 1.500000e+01, double noundef 8.000000e-01)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !106
  tail call void @codemarker_alpha(i32 noundef 48) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !107
  %10 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.38)
  %11 = tail call zeroext i1 @useResource(i32 noundef 0, double noundef 1.200000e+02)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !108
  tail call void @codemarker_alpha(i32 noundef 49) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !109
  %12 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.39)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !74
  tail call void @codemarker_alpha(i32 noundef 33) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !75
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !76
  tail call void @codemarker_alpha(i32 noundef 34) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !77
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 1.000000e+01, ptr noundef nonnull @.str.3)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !24
  tail call void @codemarker_alpha(i32 noundef 6) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !25
  %13 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !18
  %14 = fadd double %13, 1.000000e+01
  store double %14, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !18
  %15 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %16 = add nsw i32 %15, 1
  store i32 %16, ptr @totalOperations, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !28
  tail call void @codemarker_alpha(i32 noundef 8) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !29
  %17 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 1.000000e+01, ptr noundef nonnull @.str.3, double noundef %17)
  %18 = tail call zeroext i1 @useResource(i32 noundef 2, double noundef 5.000000e+01)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !74
  tail call void @codemarker_alpha(i32 noundef 33) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !75
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !76
  tail call void @codemarker_alpha(i32 noundef 34) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !77
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 4.000000e+01, ptr noundef nonnull @.str.2)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !24
  tail call void @codemarker_alpha(i32 noundef 6) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !25
  %19 = load double, ptr @resources, align 16, !tbaa !18
  %20 = fadd double %19, 4.000000e+01
  store double %20, ptr @resources, align 16, !tbaa !18
  %21 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %22 = add nsw i32 %21, 1
  store i32 %22, ptr @totalOperations, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !28
  tail call void @codemarker_alpha(i32 noundef 8) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !29
  %23 = load double, ptr @resources, align 16, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 4.000000e+01, ptr noundef nonnull @.str.2, double noundef %23)
  %24 = tail call zeroext i1 @useResource(i32 noundef 1, double noundef 2.500000e+01)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !110
  tail call void @codemarker_alpha(i32 noundef 50) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !111
  tail call void @printResources(ptr noundef nonnull @.str.34)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !112
  tail call void @codemarker_alpha(i32 noundef 51) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !113
  %25 = load i32, ptr @lastError, align 4, !tbaa !5
  %26 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.35, i32 noundef %25)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !114
  tail call void @codemarker_alpha(i32 noundef 52) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !115
  tail call void @printLogs()
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #5

attributes #0 = { noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nocallback nofree nosync nounwind willreturn }
attributes #4 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nofree nounwind }
attributes #6 = { nounwind }

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
!9 = !{i64 34524}
!10 = !{i64 2258}
!11 = !{i64 2319}
!12 = !{i64 2854}
!13 = !{i64 2915}
!14 = !{i64 3039}
!15 = !{i64 3104}
!16 = !{i64 3265}
!17 = !{i64 3326}
!18 = !{!19, !19, i64 0}
!19 = !{!"double", !7, i64 0}
!20 = !{i64 3462}
!21 = !{i64 3527}
!22 = !{i64 3821}
!23 = !{i64 3886}
!24 = !{i64 4316}
!25 = !{i64 4377}
!26 = !{i64 4501}
!27 = !{i64 4566}
!28 = !{i64 4778}
!29 = !{i64 4839}
!30 = !{i64 5472}
!31 = !{i64 5533}
!32 = !{i64 5664}
!33 = !{i64 5867}
!34 = !{i64 5978}
!35 = !{i64 6040}
!36 = !{i64 6126}
!37 = !{i64 6188}
!38 = !{i64 6677}
!39 = !{i64 6739}
!40 = !{i64 6889}
!41 = !{i64 6974}
!42 = !{i64 7044}
!43 = !{i64 7427}
!44 = !{i64 7497}
!45 = !{i64 7701}
!46 = !{i64 8285}
!47 = !{i64 8347}
!48 = !{i64 8432}
!49 = !{i64 8494}
!50 = !{i64 8768}
!51 = !{i64 8830}
!52 = !{i64 8920}
!53 = !{i64 8986}
!54 = !{i64 9124}
!55 = !{i64 9190}
!56 = !{i64 9668}
!57 = !{i64 9730}
!58 = !{i64 9350}
!59 = !{i64 9420}
!60 = !{i64 10277}
!61 = !{i64 10339}
!62 = !{i64 10418}
!63 = !{i64 10480}
!64 = !{i64 10690}
!65 = !{i64 10752}
!66 = !{i64 10929}
!67 = !{i64 10995}
!68 = !{i64 11085}
!69 = !{i64 11151}
!70 = !{i64 11270}
!71 = !{i64 11336}
!72 = !{i64 11548}
!73 = !{i64 11618}
!74 = !{i64 12148}
!75 = !{i64 12210}
!76 = !{i64 12289}
!77 = !{i64 12351}
!78 = !{i64 12725}
!79 = !{i64 12787}
!80 = !{i64 12857}
!81 = !{i64 12919}
!82 = !{i64 12982}
!83 = !{i64 13044}
!84 = !{i64 13133}
!85 = !{i64 13195}
!86 = !{i64 13290}
!87 = !{i64 13352}
!88 = !{i64 13440}
!89 = !{i64 13502}
!90 = !{i64 13674}
!91 = !{i64 13736}
!92 = !{i64 13835}
!93 = !{i64 13897}
!94 = !{i64 14118}
!95 = !{i64 14180}
!96 = !{i64 13975}
!97 = !{i64 14041}
!98 = distinct !{!98, !99}
!99 = !{!"llvm.loop.mustprogress"}
!100 = !{i64 14512}
!101 = !{i64 14574}
!102 = !{i64 14739}
!103 = !{i64 14801}
!104 = !{i64 15092}
!105 = !{i64 15154}
!106 = !{i64 15436}
!107 = !{i64 15498}
!108 = !{i64 15814}
!109 = !{i64 15876}
!110 = !{i64 16140}
!111 = !{i64 16202}
!112 = !{i64 16272}
!113 = !{i64 16334}
!114 = !{i64 16419}
!115 = !{i64 16481}

^0 = module: (path: "./file2_origin_1_LLVMIR_LINK_PREOPT/file2_origin.bc", hash: (0, 0, 0, 0, 0))
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
^14 = gv: (name: "obtainResource", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 57, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^37, tail: 1), (callee: ^57, tail: 1), (callee: ^38, tail: 1)), refs: (^54, ^40, ^47, ^43, ^35, ^33)))) ; guid = 4127045675581085826
^15 = gv: (name: "llvm.lifetime.start.p0") ; guid = 4736283418132453162
^16 = gv: (name: "lastError", summaries: (variable: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 1, constant: 0)))) ; guid = 4808874296988906674
^17 = gv: (name: ".str.5", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 5028106286570996766
^18 = gv: (name: "llvm.va_end.p0") ; guid = 5090975557526790353
^19 = gv: (name: "deductResource", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 48, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^37, tail: 1), (callee: ^57, tail: 1)), refs: (^16, ^1, ^26, ^34, ^54, ^40, ^47, ^48, ^17)))) ; guid = 5195368790120632634
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
^31 = gv: (name: "llvm.compiler.used", summaries: (variable: (module: ^0, flags: (linkage: appending, visibility: default, notEligibleToImport: 1, live: 1, dsoLocal: 0, canAutoHide: 0, importType: definition), varFlags: (readonly: 0, writeonly: 0, constant: 0), refs: (^37)))) ; guid = 9610627770985738006
^32 = gv: (name: "printLogs", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 27, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^37, tail: 1), (callee: ^25, tail: 1), (callee: ^30, tail: 1)), refs: (^13, ^11, ^5, ^21)))) ; guid = 9999713285701850125
^33 = gv: (name: ".str.14", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 10230019618414349501
^34 = gv: (name: "totalOperations", summaries: (variable: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 1, constant: 0)))) ; guid = 10287797576432768888
^35 = gv: (name: ".str.15", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 10695624196320927352
^36 = gv: (name: "str.37", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 12056547863414187638
^37 = gv: (name: "codemarker_alpha", summaries: (function: (module: ^0, flags: (linkage: internal, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 6, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 1, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0)))) ; guid = 12109312382677084523
^38 = gv: (name: "convertResourceToGetTarget", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 57, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^37, tail: 1), (callee: ^57, tail: 1), (callee: ^19, tail: 1), (callee: ^56, tail: 1)), refs: (^9, ^54, ^40, ^47, ^52, ^45, ^27, ^59)))) ; guid = 12116041630464161402
^39 = gv: (name: ".str.17", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 12691897982921208210
^40 = gv: (name: ".str.4", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 13025585774940304990
^41 = gv: (name: "llvm.lifetime.end.p0") ; guid = 13244677477227836793
^42 = gv: (name: ".str.20", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 13392787100258444056
^43 = gv: (name: ".str.13", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 14434237082905948652
^44 = gv: (name: "useResource", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 47, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^37, tail: 1), (callee: ^57, tail: 1), (callee: ^19, tail: 1), (callee: ^14, tail: 1)), refs: (^54, ^40, ^47, ^3, ^26, ^39, ^24, ^16)))) ; guid = 15323274007057276802
^45 = gv: (name: ".str.10", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 15348317746444497813
^46 = gv: (name: ".str.29", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 15493424912265585829
^47 = gv: (name: ".str.2", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 15748144372332436015
^48 = gv: (name: ".str.1", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 15757972892107832524
^49 = gv: (name: "main", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 107, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^37, tail: 1), (callee: ^58, tail: 1), (callee: ^30, tail: 1), (callee: ^44, tail: 1), (callee: ^57, tail: 1), (callee: ^38, tail: 1), (callee: ^25, tail: 1), (callee: ^32, tail: 1)), refs: (^26, ^34, ^16, ^13, ^46, ^8, ^36, ^2, ^40, ^20, ^7, ^53, ^54, ^47, ^51, ^6)))) ; guid = 15822663052811949562
^50 = gv: (name: "produceResource", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 33, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^37, tail: 1), (callee: ^57, tail: 1), (callee: ^56, tail: 1)), refs: (^2, ^47, ^26, ^34, ^20, ^54, ^40)))) ; guid = 16477953438343050136
^51 = gv: (name: ".str.34", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 16482933451644558437
^52 = gv: (name: ".str.9", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 16645872452634559675
^53 = gv: (name: "str.39", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 16852225307648081945
^54 = gv: (name: ".str.3", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 17508981887875708411
^55 = gv: (name: ".str.23", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 17592669117849567373
^56 = gv: (name: "addResource", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 32, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^37, tail: 1), (callee: ^57, tail: 1)), refs: (^16, ^23, ^26, ^34, ^54, ^40, ^47, ^20)))) ; guid = 17593996893088219275
^57 = gv: (name: "logEvent", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 20, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^37, tail: 1), (callee: ^28)), refs: (^13, ^21)))) ; guid = 18059553383448241467
^58 = gv: (name: "printResources", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 29, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^37, tail: 1), (callee: ^57, tail: 1), (callee: ^25, tail: 1)), refs: (^42, ^29, ^26, ^22, ^55, ^12, ^34, ^10)))) ; guid = 18270205028877567917
^59 = gv: (name: ".str.12", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 18378179541384066302
^60 = flags: 8
^61 = blockcount: 0
