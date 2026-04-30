; ModuleID = 'file2_origin_1_CLANGOPT.out.0.5.precodegen.bc'
source_filename = "ld-temp.o"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.__va_list_tag = type { i32, i32, ptr, ptr }

@totalOperations = internal unnamed_addr global i32 0, align 4
@lastError = internal unnamed_addr global i32 0, align 4
@logCount = internal unnamed_addr global i32 0, align 4
@logMessages = internal global [100 x [256 x i8]] zeroinitializer, align 16
@.str = private unnamed_addr constant [54 x i8] c"ERROR: Invalid deductResource call (res=%d, amt=%.2f)\00", align 1
@resources = internal unnamed_addr global [3 x double] zeroinitializer, align 16
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
define internal void @logEvent(ptr noundef readonly captures(none) %0, ...) unnamed_addr #2 {
  %2 = alloca [1 x %struct.__va_list_tag], align 16
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !10
  tail call void @codemarker_alpha(i32 noundef 0) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !11
  %3 = load i32, ptr @logCount, align 4, !tbaa !5
  %4 = icmp sgt i32 %3, 99
  br i1 %4, label %11, label %5

5:                                                ; preds = %1
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %2) #6
  call void @llvm.va_start.p0(ptr nonnull %2)
  %6 = sext i32 %3 to i64
  %7 = getelementptr inbounds [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %6
  %8 = call i32 @vsnprintf(ptr noundef nonnull %7, i64 noundef 256, ptr noundef %0, ptr noundef nonnull %2) #6
  call void @llvm.va_end.p0(ptr nonnull %2)
  %9 = load i32, ptr @logCount, align 4, !tbaa !5
  %10 = add nsw i32 %9, 1
  store i32 %10, ptr @logCount, align 4, !tbaa !5
  call void @llvm.lifetime.end.p0(i64 24, ptr nonnull %2) #6
  br label %11

11:                                               ; preds = %5, %1
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #3

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @vsnprintf(ptr noundef captures(none), i64 noundef, ptr noundef readonly captures(none), ptr noundef) local_unnamed_addr #4

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end.p0(ptr) #3

; Function Attrs: nounwind uwtable
define internal fastcc noundef zeroext i1 @deductResource(i32 noundef range(i32 0, 3) %0, double noundef %1) unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !12
  tail call void @codemarker_alpha(i32 noundef 1) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !13
  %3 = fcmp olt double %1, 0.000000e+00
  br i1 %3, label %4, label %5

4:                                                ; preds = %2
  store i32 1, ptr @lastError, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !14
  tail call void @codemarker_alpha(i32 noundef 2) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !15
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str, i32 noundef %0, double noundef %1)
  br label %25

5:                                                ; preds = %2
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !16
  tail call void @codemarker_alpha(i32 noundef 3) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !17
  %6 = zext nneg i32 %0 to i64
  %7 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %6
  %8 = load double, ptr %7, align 8, !tbaa !18
  %9 = fcmp ult double %8, %1
  br i1 %9, label %19, label %10

10:                                               ; preds = %5
  %11 = fsub double %8, %1
  store double %11, ptr %7, align 8, !tbaa !18
  %12 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %13 = add nsw i32 %12, 1
  store i32 %13, ptr @totalOperations, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !20
  tail call void @codemarker_alpha(i32 noundef 4) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !21
  %14 = icmp eq i32 %0, 0
  %15 = icmp eq i32 %0, 1
  %16 = select i1 %15, ptr @.str.3, ptr @.str.4
  %17 = select i1 %14, ptr @.str.2, ptr %16
  %18 = load double, ptr %7, align 8, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %1, ptr noundef nonnull %17, double noundef %18)
  br label %25

19:                                               ; preds = %5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !22
  tail call void @codemarker_alpha(i32 noundef 5) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !23
  %20 = icmp eq i32 %0, 0
  %21 = icmp eq i32 %0, 1
  %22 = select i1 %21, ptr @.str.3, ptr @.str.4
  %23 = select i1 %20, ptr @.str.2, ptr %22
  %24 = load double, ptr %7, align 8, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.5, ptr noundef nonnull %23, double noundef %24, double noundef %1)
  br label %25

25:                                               ; preds = %19, %10, %4
  %26 = phi i1 [ false, %4 ], [ true, %10 ], [ false, %19 ]
  ret i1 %26
}

; Function Attrs: nounwind uwtable
define internal fastcc noundef zeroext i1 @addResource(i32 noundef range(i32 0, 3) %0, double noundef %1) unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !24
  tail call void @codemarker_alpha(i32 noundef 6) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !25
  %3 = fcmp uge double %1, 0.000000e+00
  br i1 %3, label %5, label %4

4:                                                ; preds = %2
  store i32 2, ptr @lastError, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !26
  tail call void @codemarker_alpha(i32 noundef 7) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !27
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.6, i32 noundef %0, double noundef %1)
  br label %17

5:                                                ; preds = %2
  %6 = zext nneg i32 %0 to i64
  %7 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %6
  %8 = load double, ptr %7, align 8, !tbaa !18
  %9 = fadd double %1, %8
  store double %9, ptr %7, align 8, !tbaa !18
  %10 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr @totalOperations, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !28
  tail call void @codemarker_alpha(i32 noundef 8) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !29
  %12 = icmp eq i32 %0, 0
  %13 = icmp eq i32 %0, 1
  %14 = select i1 %13, ptr @.str.3, ptr @.str.4
  %15 = select i1 %12, ptr @.str.2, ptr %14
  %16 = load double, ptr %7, align 8, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %1, ptr noundef nonnull %15, double noundef %16)
  br label %17

17:                                               ; preds = %5, %4
  ret i1 %3
}

; Function Attrs: nounwind uwtable
define internal fastcc noundef zeroext i1 @convertResourceToGetTarget(i32 noundef range(i32 0, 3) %0, i32 noundef range(i32 0, 3) %1, double noundef %2) unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !30
  tail call void @codemarker_alpha(i32 noundef 9) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !31
  %4 = icmp eq i32 %0, %1
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6
  br i1 %4, label %5, label %6

5:                                                ; preds = %3
  tail call void @codemarker_alpha(i32 noundef 10) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !32
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.8)
  br label %25

6:                                                ; preds = %3
  tail call void @codemarker_alpha(i32 noundef 11) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !33
  %7 = fcmp ugt double %2, 0.000000e+00
  br i1 %7, label %8, label %25

8:                                                ; preds = %6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !34
  tail call void @codemarker_alpha(i32 noundef 12) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !35
  %9 = fdiv double %2, 8.000000e-01
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !36
  tail call void @codemarker_alpha(i32 noundef 13) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !37
  %10 = icmp eq i32 %0, 0
  %11 = icmp eq i32 %0, 1
  %12 = select i1 %11, ptr @.str.3, ptr @.str.4
  %13 = select i1 %10, ptr @.str.2, ptr %12
  %14 = icmp eq i32 %1, 0
  %15 = icmp eq i32 %1, 1
  %16 = select i1 %15, ptr @.str.3, ptr @.str.4
  %17 = select i1 %14, ptr @.str.2, ptr %16
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.9, ptr noundef nonnull %13, ptr noundef nonnull %17, double noundef %2, double noundef %9)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !38
  tail call void @codemarker_alpha(i32 noundef 14) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !39
  %18 = tail call fastcc zeroext i1 @deductResource(i32 noundef %0, double noundef %9)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6
  br i1 %18, label %19, label %24

19:                                               ; preds = %8
  tail call void @codemarker_alpha(i32 noundef 15) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !40
  %20 = tail call fastcc zeroext i1 @addResource(i32 noundef %1, double noundef %2)
  br i1 %20, label %21, label %22

21:                                               ; preds = %19
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !41
  tail call void @codemarker_alpha(i32 noundef 16) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !42
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.10, double noundef %9, double noundef %2)
  br label %25

22:                                               ; preds = %19
  %23 = tail call fastcc zeroext i1 @addResource(i32 noundef %0, double noundef %9)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !43
  tail call void @codemarker_alpha(i32 noundef 17) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !44
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.11)
  br label %25

24:                                               ; preds = %8
  tail call void @codemarker_alpha(i32 noundef 18) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !45
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.12, ptr noundef nonnull %13)
  br label %25

25:                                               ; preds = %24, %22, %21, %6, %5
  %26 = phi i1 [ false, %5 ], [ true, %6 ], [ true, %21 ], [ false, %22 ], [ false, %24 ]
  ret i1 %26
}

; Function Attrs: nounwind uwtable
define internal fastcc void @useResource(i32 noundef range(i32 0, 3) %0, double noundef %1) unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !46
  tail call void @codemarker_alpha(i32 noundef 26) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !47
  %3 = fcmp ugt double %1, 0.000000e+00
  br i1 %3, label %4, label %34

4:                                                ; preds = %2
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !48
  tail call void @codemarker_alpha(i32 noundef 27) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !49
  %5 = icmp eq i32 %0, 0
  %6 = icmp eq i32 %0, 1
  %7 = select i1 %6, ptr @.str.3, ptr @.str.4
  %8 = select i1 %5, ptr @.str.2, ptr %7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.16, double noundef %1, ptr noundef nonnull %8)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !50
  tail call void @codemarker_alpha(i32 noundef 28) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !51
  %9 = zext nneg i32 %0 to i64
  %10 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %9
  %11 = load double, ptr %10, align 8, !tbaa !18
  %12 = fcmp ult double %11, %1
  br i1 %12, label %15, label %13

13:                                               ; preds = %4
  %14 = tail call fastcc zeroext i1 @deductResource(i32 noundef %0, double noundef %1)
  br label %34

15:                                               ; preds = %4
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !52
  tail call void @codemarker_alpha(i32 noundef 29) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !53
  %16 = load double, ptr %10, align 8, !tbaa !18
  %17 = fsub double %1, %16
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !54
  tail call void @codemarker_alpha(i32 noundef 30) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !55
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.17, double noundef %17)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !56
  tail call void @codemarker_alpha(i32 noundef 31) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !57
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !58
  tail call void @codemarker_alpha(i32 noundef 19) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !59
  %18 = fcmp ugt double %17, 0.000000e+00
  br i1 %18, label %19, label %31

19:                                               ; preds = %15
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !60
  tail call void @codemarker_alpha(i32 noundef 20) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !61
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.13, double noundef %17, ptr noundef nonnull %8)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !62
  tail call void @codemarker_alpha(i32 noundef 21) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !63
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !64
  tail call void @codemarker_alpha(i32 noundef 22) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !65
  br i1 %5, label %22, label %20

20:                                               ; preds = %19
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !66
  tail call void @codemarker_alpha(i32 noundef 23) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !67
  %21 = tail call fastcc zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef range(i32 0, 3) %0, double noundef %17)
  br i1 %21, label %29, label %22

22:                                               ; preds = %20, %19
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !64
  tail call void @codemarker_alpha(i32 noundef 22) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !65
  br i1 %6, label %25, label %23

23:                                               ; preds = %22
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !66
  tail call void @codemarker_alpha(i32 noundef 23) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !67
  %24 = tail call fastcc zeroext i1 @convertResourceToGetTarget(i32 noundef 1, i32 noundef range(i32 0, 3) %0, double noundef %17)
  br i1 %24, label %29, label %25

25:                                               ; preds = %23, %22
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !64
  tail call void @codemarker_alpha(i32 noundef 22) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !65
  %26 = icmp eq i32 %0, 2
  br i1 %26, label %33, label %27

27:                                               ; preds = %25
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !66
  tail call void @codemarker_alpha(i32 noundef 23) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !67
  %28 = tail call fastcc zeroext i1 @convertResourceToGetTarget(i32 noundef 2, i32 noundef range(i32 0, 3) %0, double noundef %17)
  br i1 %28, label %29, label %33

29:                                               ; preds = %27, %23, %20
  %30 = phi ptr [ @.str.2, %20 ], [ @.str.3, %23 ], [ @.str.4, %27 ]
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !68
  tail call void @codemarker_alpha(i32 noundef 24) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !69
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.14, ptr noundef nonnull %30)
  br label %31

31:                                               ; preds = %29, %15
  %32 = tail call fastcc zeroext i1 @deductResource(i32 noundef %0, double noundef %1)
  br label %34

33:                                               ; preds = %27, %25
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !70
  tail call void @codemarker_alpha(i32 noundef 25) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !71
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.15, double noundef %17, ptr noundef nonnull %8)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !72
  tail call void @codemarker_alpha(i32 noundef 32) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !73
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.18, double noundef %1, ptr noundef nonnull %8)
  store i32 3, ptr @lastError, align 4, !tbaa !5
  br label %34

34:                                               ; preds = %33, %31, %13, %2
  ret void
}

; Function Attrs: nounwind uwtable
define internal fastcc void @printResources(ptr noundef %0) unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !74
  tail call void @codemarker_alpha(i32 noundef 35) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !75
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.20, ptr noundef %0)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !76
  tail call void @codemarker_alpha(i32 noundef 36) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !77
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.21, ptr noundef %0)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !78
  tail call void @codemarker_alpha(i32 noundef 37) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !79
  %3 = load double, ptr @resources, align 16, !tbaa !18
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.22, double noundef %3)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !80
  tail call void @codemarker_alpha(i32 noundef 38) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !81
  %5 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !18
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.23, double noundef %5)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !82
  tail call void @codemarker_alpha(i32 noundef 39) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !83
  %7 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !18
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.24, double noundef %7)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !84
  tail call void @codemarker_alpha(i32 noundef 40) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !85
  %9 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %10 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.25, i32 noundef %9)
  ret void
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #4

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #5

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #2 {
  store <2 x double> <double 1.000000e+02, double 5.000000e+01>, ptr @resources, align 16, !tbaa !18
  store double 8.000000e+01, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !18
  store i32 0, ptr @totalOperations, align 4, !tbaa !5
  store i32 0, ptr @lastError, align 4, !tbaa !5
  store i32 0, ptr @logCount, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !86
  tail call void @codemarker_alpha(i32 noundef 45) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !87
  tail call fastcc void @printResources(ptr noundef nonnull @.str.29)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !88
  tail call void @codemarker_alpha(i32 noundef 46) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !89
  %1 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.36)
  tail call fastcc void @useResource(i32 noundef 1, double noundef 7.000000e+01)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !90
  tail call void @codemarker_alpha(i32 noundef 47) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !91
  %2 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.37)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !92
  tail call void @codemarker_alpha(i32 noundef 33) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !93
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !94
  tail call void @codemarker_alpha(i32 noundef 34) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !95
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 3.000000e+01, ptr noundef nonnull @.str.4)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !24
  tail call void @codemarker_alpha(i32 noundef 6) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !25
  %3 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !18
  %4 = fadd double %3, 3.000000e+01
  store double %4, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !18
  %5 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %6 = add nsw i32 %5, 1
  store i32 %6, ptr @totalOperations, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !28
  tail call void @codemarker_alpha(i32 noundef 8) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !29
  %7 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 3.000000e+01, ptr noundef nonnull @.str.4, double noundef %7)
  %8 = tail call fastcc zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef 1, double noundef 1.500000e+01)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !96
  tail call void @codemarker_alpha(i32 noundef 48) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !97
  %9 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.38)
  tail call fastcc void @useResource(i32 noundef 0, double noundef 1.200000e+02)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !98
  tail call void @codemarker_alpha(i32 noundef 49) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !99
  %10 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.39)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !92
  tail call void @codemarker_alpha(i32 noundef 33) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !93
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !94
  tail call void @codemarker_alpha(i32 noundef 34) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !95
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 1.000000e+01, ptr noundef nonnull @.str.3)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !24
  tail call void @codemarker_alpha(i32 noundef 6) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !25
  %11 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !18
  %12 = fadd double %11, 1.000000e+01
  store double %12, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !18
  %13 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %14 = add nsw i32 %13, 1
  store i32 %14, ptr @totalOperations, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !28
  tail call void @codemarker_alpha(i32 noundef 8) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !29
  %15 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 1.000000e+01, ptr noundef nonnull @.str.3, double noundef %15)
  tail call fastcc void @useResource(i32 noundef 2, double noundef 5.000000e+01)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !92
  tail call void @codemarker_alpha(i32 noundef 33) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !93
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !94
  tail call void @codemarker_alpha(i32 noundef 34) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !95
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 4.000000e+01, ptr noundef nonnull @.str.2)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !24
  tail call void @codemarker_alpha(i32 noundef 6) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !25
  %16 = load double, ptr @resources, align 16, !tbaa !18
  %17 = fadd double %16, 4.000000e+01
  store double %17, ptr @resources, align 16, !tbaa !18
  %18 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %19 = add nsw i32 %18, 1
  store i32 %19, ptr @totalOperations, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !28
  tail call void @codemarker_alpha(i32 noundef 8) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !29
  %20 = load double, ptr @resources, align 16, !tbaa !18
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 4.000000e+01, ptr noundef nonnull @.str.2, double noundef %20)
  tail call fastcc void @useResource(i32 noundef 1, double noundef 2.500000e+01)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !100
  tail call void @codemarker_alpha(i32 noundef 50) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !101
  tail call fastcc void @printResources(ptr noundef nonnull @.str.34)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !102
  tail call void @codemarker_alpha(i32 noundef 51) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !103
  %21 = load i32, ptr @lastError, align 4, !tbaa !5
  %22 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.35, i32 noundef %21)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !104
  tail call void @codemarker_alpha(i32 noundef 52) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !105
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !106
  tail call void @codemarker_alpha(i32 noundef 41) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !107
  %23 = load i32, ptr @logCount, align 4, !tbaa !5
  %24 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.26, i32 noundef %23)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !108
  tail call void @codemarker_alpha(i32 noundef 42) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !109
  %25 = load i32, ptr @logCount, align 4, !tbaa !5
  %26 = icmp sgt i32 %25, 0
  br i1 %26, label %.preheader, label %printLogs.exit

.preheader:                                       ; preds = %.preheader, %0
  %27 = phi i64 [ %30, %.preheader ], [ 0, %0 ]
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !110
  tail call void @codemarker_alpha(i32 noundef 43) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !111
  %28 = getelementptr inbounds nuw [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %27
  %29 = tail call i32 @puts(ptr nonnull dereferenceable(1) %28)
  %30 = add nuw nsw i64 %27, 1
  %31 = load i32, ptr @logCount, align 4, !tbaa !5
  %32 = sext i32 %31 to i64
  %33 = icmp slt i64 %30, %32
  br i1 %33, label %.preheader, label %printLogs.exit, !llvm.loop !112

printLogs.exit:                                   ; preds = %.preheader, %0
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !114
  tail call void @codemarker_alpha(i32 noundef 44) #6
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #6, !srcloc !115
  %34 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nocallback nofree nosync nounwind willreturn }
attributes #4 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nofree nounwind }
attributes #6 = { nounwind }

!llvm.ident = !{!0}
!llvm.module.flags = !{!1, !2, !3, !4}

!0 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 1, !"ThinLTO", i32 0}
!4 = !{i32 1, !"EnableSplitLTOUnit", i32 1}
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
!46 = !{i64 10277}
!47 = !{i64 10339}
!48 = !{i64 10418}
!49 = !{i64 10480}
!50 = !{i64 10690}
!51 = !{i64 10752}
!52 = !{i64 10929}
!53 = !{i64 10995}
!54 = !{i64 11085}
!55 = !{i64 11151}
!56 = !{i64 11270}
!57 = !{i64 11336}
!58 = !{i64 8285}
!59 = !{i64 8347}
!60 = !{i64 8432}
!61 = !{i64 8494}
!62 = !{i64 8768}
!63 = !{i64 8830}
!64 = !{i64 8920}
!65 = !{i64 8986}
!66 = !{i64 9124}
!67 = !{i64 9190}
!68 = !{i64 9350}
!69 = !{i64 9420}
!70 = !{i64 9668}
!71 = !{i64 9730}
!72 = !{i64 11548}
!73 = !{i64 11618}
!74 = !{i64 12725}
!75 = !{i64 12787}
!76 = !{i64 12857}
!77 = !{i64 12919}
!78 = !{i64 12982}
!79 = !{i64 13044}
!80 = !{i64 13133}
!81 = !{i64 13195}
!82 = !{i64 13290}
!83 = !{i64 13352}
!84 = !{i64 13440}
!85 = !{i64 13502}
!86 = !{i64 14512}
!87 = !{i64 14574}
!88 = !{i64 14739}
!89 = !{i64 14801}
!90 = !{i64 15092}
!91 = !{i64 15154}
!92 = !{i64 12148}
!93 = !{i64 12210}
!94 = !{i64 12289}
!95 = !{i64 12351}
!96 = !{i64 15436}
!97 = !{i64 15498}
!98 = !{i64 15814}
!99 = !{i64 15876}
!100 = !{i64 16140}
!101 = !{i64 16202}
!102 = !{i64 16272}
!103 = !{i64 16334}
!104 = !{i64 16419}
!105 = !{i64 16481}
!106 = !{i64 13674}
!107 = !{i64 13736}
!108 = !{i64 13835}
!109 = !{i64 13897}
!110 = !{i64 13975}
!111 = !{i64 14041}
!112 = distinct !{!112, !113}
!113 = !{!"llvm.loop.mustprogress"}
!114 = !{i64 14118}
!115 = !{i64 14180}
