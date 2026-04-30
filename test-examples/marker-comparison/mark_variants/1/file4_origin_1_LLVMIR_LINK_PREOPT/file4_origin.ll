; ModuleID = './file4_origin_1_LLVMIR_LINK_PREOPT/file4_origin.bc'
source_filename = "file4_origin.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@.str = private unnamed_addr constant [27 x i8] c"compute_something(5) = %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [22 x i8] c"constant_fold() = %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [28 x i8] c"branch_elimination(0) = %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [23 x i8] c"cse_example(3,4) = %d\0A\00", align 1
@.str.4 = private unnamed_addr constant [30 x i8] c"inline_candidate(10,20) = %d\0A\00", align 1
@.str.5 = private unnamed_addr constant [19 x i8] c"max(100,200) = %d\0A\00", align 1
@.str.6 = private unnamed_addr constant [16 x i8] c"grade(85) = %c\0A\00", align 1
@.str.7 = private unnamed_addr constant [20 x i8] c"algebraic(42) = %d\0A\00", align 1
@.str.8 = private unnamed_addr constant [20 x i8] c"bit_tricks(7) = %d\0A\00", align 1
@__const.main.src = private unnamed_addr constant [32 x i8] c"Hello optimizable world!\00\00\00\00\00\00\00\00", align 16
@.str.9 = private unnamed_addr constant [12 x i8] c"Copied: %s\0A\00", align 1
@.str.10 = private unnamed_addr constant [50 x i8] c"square = %d, add_five = %d, multiply_by_two = %d\0A\00", align 1
@llvm.compiler.used = appending global [1 x ptr] [ptr @codemarker_alpha], section "llvm.metadata"

; Function Attrs: noinline nounwind uwtable
define internal void @codemarker_alpha(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  call void @llvm.lifetime.start.p0(i64 4, ptr nonnull %2)
  store volatile i32 %0, ptr %2, align 4, !tbaa !5
  tail call void asm sideeffect "", "r,~{memory},~{dirflag},~{fpsr},~{flags}"(i32 %0) #7, !srcloc !9
  call void @llvm.lifetime.end.p0(i64 4, ptr nonnull %2)
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef range(i32 0, -1) i32 @multiply_by_two(i32 noundef %0) local_unnamed_addr #2 {
  %2 = shl i32 %0, 1
  ret i32 %2
}

; Function Attrs: nounwind uwtable
define dso_local void @init_array(ptr noundef writeonly captures(none) %0, i32 noundef %1) local_unnamed_addr #3 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !10
  tail call void @codemarker_alpha(i32 noundef 0) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !11
  %3 = icmp sgt i32 %1, 0
  br i1 %3, label %4, label %24

4:                                                ; preds = %2
  %5 = zext nneg i32 %1 to i64
  %6 = icmp ult i32 %1, 8
  br i1 %6, label %22, label %7

7:                                                ; preds = %4
  %8 = and i64 %5, 2147483640
  br label %9

9:                                                ; preds = %9, %7
  %10 = phi i64 [ 0, %7 ], [ %17, %9 ]
  %11 = phi <4 x i32> [ <i32 0, i32 1, i32 2, i32 3>, %7 ], [ %18, %9 ]
  %12 = getelementptr inbounds nuw i32, ptr %0, i64 %10
  %13 = mul <4 x i32> %11, splat (i32 3)
  %14 = mul <4 x i32> %11, splat (i32 3)
  %15 = add <4 x i32> %14, splat (i32 12)
  %16 = getelementptr inbounds nuw i8, ptr %12, i64 16
  store <4 x i32> %13, ptr %12, align 4, !tbaa !5
  store <4 x i32> %15, ptr %16, align 4, !tbaa !5
  %17 = add nuw i64 %10, 8
  %18 = add <4 x i32> %11, splat (i32 8)
  %19 = icmp eq i64 %17, %8
  br i1 %19, label %20, label %9, !llvm.loop !12

20:                                               ; preds = %9
  %21 = icmp eq i64 %8, %5
  br i1 %21, label %24, label %22

22:                                               ; preds = %20, %4
  %23 = phi i64 [ 0, %4 ], [ %8, %20 ]
  br label %25

24:                                               ; preds = %25, %20, %2
  ret void

25:                                               ; preds = %25, %22
  %26 = phi i64 [ %30, %25 ], [ %23, %22 ]
  %27 = getelementptr inbounds nuw i32, ptr %0, i64 %26
  %28 = trunc i64 %26 to i32
  %29 = mul i32 %28, 3
  store i32 %29, ptr %27, align 4, !tbaa !5
  %30 = add nuw nsw i64 %26, 1
  %31 = icmp eq i64 %30, %5
  br i1 %31, label %24, label %25, !llvm.loop !16
}

; Function Attrs: nounwind uwtable
define dso_local range(i32 -2147483648, 2147483645) i32 @compute_something(i32 noundef %0) local_unnamed_addr #3 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !17
  tail call void @codemarker_alpha(i32 noundef 1) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !18
  %2 = shl i32 %0, 1
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !19
  tail call void @codemarker_alpha(i32 noundef 2) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !20
  %3 = add i32 %2, 17
  ret i32 %3
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @constant_fold() local_unnamed_addr #3 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !21
  tail call void @codemarker_alpha(i32 noundef 3) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !22
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !23
  tail call void @codemarker_alpha(i32 noundef 4) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !24
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !25
  tail call void @codemarker_alpha(i32 noundef 5) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !26
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !27
  tail call void @codemarker_alpha(i32 noundef 6) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !28
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !29
  tail call void @codemarker_alpha(i32 noundef 7) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !30
  ret i32 20
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @branch_elimination(i32 noundef %0) local_unnamed_addr #3 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !31
  tail call void @codemarker_alpha(i32 noundef 8) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !32
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !33
  tail call void @codemarker_alpha(i32 noundef 9) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !34
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !35
  tail call void @codemarker_alpha(i32 noundef 10) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !36
  ret i32 48
}

; Function Attrs: nounwind uwtable
define dso_local range(i32 -2147483648, 2147483647) i32 @cse_example(i32 noundef %0, i32 noundef %1) local_unnamed_addr #3 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !37
  tail call void @codemarker_alpha(i32 noundef 11) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !38
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !39
  tail call void @codemarker_alpha(i32 noundef 12) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !40
  %3 = shl i32 %0, 1
  %4 = mul i32 %3, %1
  ret i32 %4
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local range(i32 -2147483548, -2147483648) i32 @inline_candidate(i32 noundef %0, i32 noundef %1) local_unnamed_addr #2 {
  %3 = tail call noundef i32 @llvm.smax.i32(i32 %0, i32 %1)
  %4 = add nsw i32 %3, 100
  ret i32 %4
}

; Function Attrs: nounwind uwtable
define dso_local i32 @loop_invariant(ptr noundef readonly captures(none) %0, i32 noundef %1, i32 noundef %2) local_unnamed_addr #3 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !41
  tail call void @codemarker_alpha(i32 noundef 13) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !42
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !43
  tail call void @codemarker_alpha(i32 noundef 14) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !44
  %4 = icmp sgt i32 %1, 0
  br i1 %4, label %5, label %33

5:                                                ; preds = %3
  %6 = zext nneg i32 %1 to i64
  %7 = icmp ult i32 %1, 8
  br i1 %7, label %30, label %8

8:                                                ; preds = %5
  %9 = and i64 %6, 2147483640
  %10 = insertelement <4 x i32> poison, i32 %2, i64 0
  %11 = shufflevector <4 x i32> %10, <4 x i32> poison, <4 x i32> zeroinitializer
  br label %12

12:                                               ; preds = %12, %8
  %13 = phi i64 [ 0, %8 ], [ %24, %12 ]
  %14 = phi <4 x i32> [ zeroinitializer, %8 ], [ %22, %12 ]
  %15 = phi <4 x i32> [ zeroinitializer, %8 ], [ %23, %12 ]
  %16 = getelementptr inbounds nuw i32, ptr %0, i64 %13
  %17 = getelementptr inbounds nuw i8, ptr %16, i64 16
  %18 = load <4 x i32>, ptr %16, align 4, !tbaa !5
  %19 = load <4 x i32>, ptr %17, align 4, !tbaa !5
  %20 = mul nsw <4 x i32> %18, %11
  %21 = mul nsw <4 x i32> %19, %11
  %22 = add <4 x i32> %20, %14
  %23 = add <4 x i32> %21, %15
  %24 = add nuw i64 %13, 8
  %25 = icmp eq i64 %24, %9
  br i1 %25, label %26, label %12, !llvm.loop !45

26:                                               ; preds = %12
  %27 = add <4 x i32> %23, %22
  %28 = tail call i32 @llvm.vector.reduce.add.v4i32(<4 x i32> %27)
  %29 = icmp eq i64 %9, %6
  br i1 %29, label %33, label %30

30:                                               ; preds = %26, %5
  %31 = phi i64 [ 0, %5 ], [ %9, %26 ]
  %32 = phi i32 [ 0, %5 ], [ %28, %26 ]
  br label %35

33:                                               ; preds = %35, %26, %3
  %34 = phi i32 [ 0, %3 ], [ %28, %26 ], [ %41, %35 ]
  ret i32 %34

35:                                               ; preds = %35, %30
  %36 = phi i64 [ %42, %35 ], [ %31, %30 ]
  %37 = phi i32 [ %41, %35 ], [ %32, %30 ]
  %38 = getelementptr inbounds nuw i32, ptr %0, i64 %36
  %39 = load i32, ptr %38, align 4, !tbaa !5
  %40 = mul nsw i32 %39, %2
  %41 = add nsw i32 %40, %37
  %42 = add nuw nsw i64 %36, 1
  %43 = icmp eq i64 %42, %6
  br i1 %43, label %33, label %35, !llvm.loop !46
}

; Function Attrs: nounwind uwtable
define dso_local void @strength_reduce(ptr noundef writeonly captures(none) %0, i32 noundef %1) local_unnamed_addr #3 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !47
  tail call void @codemarker_alpha(i32 noundef 15) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !48
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !49
  tail call void @codemarker_alpha(i32 noundef 16) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !50
  %3 = icmp sgt i32 %1, 0
  br i1 %3, label %4, label %25

4:                                                ; preds = %2
  %5 = zext nneg i32 %1 to i64
  %6 = icmp ult i32 %1, 8
  br i1 %6, label %22, label %7

7:                                                ; preds = %4
  %8 = and i64 %5, 2147483640
  %9 = trunc nuw nsw i64 %8 to i32
  %10 = shl nuw i32 %9, 1
  br label %11

11:                                               ; preds = %11, %7
  %12 = phi i64 [ 0, %7 ], [ %17, %11 ]
  %13 = phi <4 x i32> [ <i32 0, i32 2, i32 4, i32 6>, %7 ], [ %18, %11 ]
  %14 = add <4 x i32> %13, splat (i32 8)
  %15 = getelementptr inbounds nuw i32, ptr %0, i64 %12
  %16 = getelementptr inbounds nuw i8, ptr %15, i64 16
  store <4 x i32> %13, ptr %15, align 4, !tbaa !5
  store <4 x i32> %14, ptr %16, align 4, !tbaa !5
  %17 = add nuw i64 %12, 8
  %18 = add <4 x i32> %13, splat (i32 16)
  %19 = icmp eq i64 %17, %8
  br i1 %19, label %20, label %11, !llvm.loop !51

20:                                               ; preds = %11
  %21 = icmp eq i64 %8, %5
  br i1 %21, label %25, label %22

22:                                               ; preds = %20, %4
  %23 = phi i64 [ 0, %4 ], [ %8, %20 ]
  %24 = phi i32 [ 0, %4 ], [ %10, %20 ]
  br label %26

25:                                               ; preds = %26, %20, %2
  ret void

26:                                               ; preds = %26, %22
  %27 = phi i64 [ %31, %26 ], [ %23, %22 ]
  %28 = phi i32 [ %30, %26 ], [ %24, %22 ]
  %29 = getelementptr inbounds nuw i32, ptr %0, i64 %27
  store i32 %28, ptr %29, align 4, !tbaa !5
  %30 = add nuw nsw i32 %28, 2
  %31 = add nuw nsw i64 %27, 1
  %32 = icmp eq i64 %31, %5
  br i1 %32, label %25, label %26, !llvm.loop !52
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local i32 @dead_code(i32 noundef %0) local_unnamed_addr #2 {
  %2 = mul nsw i32 %0, %0
  ret i32 %2
}

; Function Attrs: nounwind uwtable
define dso_local i32 @store_load(ptr noundef captures(none) initializes((0, 4)) %0) local_unnamed_addr #3 {
  store i32 10, ptr %0, align 4, !tbaa !5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !53
  tail call void @codemarker_alpha(i32 noundef 18) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !54
  %2 = load i32, ptr %0, align 4, !tbaa !5
  ret i32 %2
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @max(i32 noundef %0, i32 noundef %1) local_unnamed_addr #3 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !55
  tail call void @codemarker_alpha(i32 noundef 19) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !56
  %3 = tail call i32 @llvm.smax.i32(i32 %0, i32 %1)
  ret i32 %3
}

; Function Attrs: nounwind uwtable
define dso_local signext range(i8 65, 71) i8 @grade(i32 noundef %0) local_unnamed_addr #3 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !57
  tail call void @codemarker_alpha(i32 noundef 20) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !58
  %2 = sdiv i32 %0, 10
  switch i32 %2, label %7 [
    i32 10, label %3
    i32 9, label %8
    i32 8, label %4
    i32 7, label %5
    i32 6, label %6
  ]

3:                                                ; preds = %1
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !59
  tail call void @codemarker_alpha(i32 noundef 22) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !60
  br label %8

4:                                                ; preds = %1
  br label %8

5:                                                ; preds = %1
  br label %8

6:                                                ; preds = %1
  br label %8

7:                                                ; preds = %1
  br label %8

8:                                                ; preds = %7, %6, %5, %4, %3, %1
  %9 = phi i8 [ 70, %7 ], [ 66, %4 ], [ 67, %5 ], [ 68, %6 ], [ 65, %1 ], [ 65, %3 ]
  ret i8 %9
}

; Function Attrs: nounwind uwtable
define dso_local void @copy_buffer(ptr noundef writeonly captures(none) initializes((0, 32)) %0, ptr noundef readonly captures(none) %1) local_unnamed_addr #3 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !61
  tail call void @codemarker_alpha(i32 noundef 26) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !62
  tail call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 1 dereferenceable(32) %0, ptr noundef nonnull align 1 dereferenceable(32) %1, i64 32, i1 false)
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #4

; Function Attrs: nounwind uwtable
define dso_local void @loop_fusion(ptr noundef captures(none) %0, ptr noundef writeonly captures(none) %1, i32 noundef %2) local_unnamed_addr #3 {
  %4 = ptrtoint ptr %0 to i64
  %5 = ptrtoint ptr %1 to i64
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !63
  tail call void @codemarker_alpha(i32 noundef 27) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !64
  %6 = icmp sgt i32 %2, 0
  br i1 %6, label %7, label %71

7:                                                ; preds = %3
  %8 = zext nneg i32 %2 to i64
  %9 = icmp ult i32 %2, 8
  br i1 %9, label %23, label %10

10:                                               ; preds = %7
  %11 = and i64 %8, 2147483640
  br label %12

12:                                               ; preds = %12, %10
  %13 = phi i64 [ 0, %10 ], [ %18, %12 ]
  %14 = phi <4 x i32> [ <i32 0, i32 1, i32 2, i32 3>, %10 ], [ %19, %12 ]
  %15 = add <4 x i32> %14, splat (i32 4)
  %16 = getelementptr inbounds nuw i32, ptr %0, i64 %13
  %17 = getelementptr inbounds nuw i8, ptr %16, i64 16
  store <4 x i32> %14, ptr %16, align 4, !tbaa !5
  store <4 x i32> %15, ptr %17, align 4, !tbaa !5
  %18 = add nuw i64 %13, 8
  %19 = add <4 x i32> %14, splat (i32 8)
  %20 = icmp eq i64 %18, %11
  br i1 %20, label %21, label %12, !llvm.loop !65

21:                                               ; preds = %12
  %22 = icmp eq i64 %11, %8
  br i1 %22, label %25, label %23

23:                                               ; preds = %21, %7
  %24 = phi i64 [ 0, %7 ], [ %11, %21 ]
  br label %65

25:                                               ; preds = %65, %21
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !66
  tail call void @codemarker_alpha(i32 noundef 28) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !67
  %26 = zext nneg i32 %2 to i64
  %27 = icmp ult i32 %2, 8
  %28 = sub i64 %5, %4
  %29 = icmp ult i64 %28, 32
  %30 = or i1 %27, %29
  br i1 %30, label %47, label %31

31:                                               ; preds = %25
  %32 = and i64 %8, 2147483640
  br label %33

33:                                               ; preds = %33, %31
  %34 = phi i64 [ 0, %31 ], [ %43, %33 ]
  %35 = getelementptr inbounds nuw i32, ptr %0, i64 %34
  %36 = getelementptr inbounds nuw i8, ptr %35, i64 16
  %37 = load <4 x i32>, ptr %35, align 4, !tbaa !5
  %38 = load <4 x i32>, ptr %36, align 4, !tbaa !5
  %39 = shl nsw <4 x i32> %37, splat (i32 1)
  %40 = shl nsw <4 x i32> %38, splat (i32 1)
  %41 = getelementptr inbounds nuw i32, ptr %1, i64 %34
  %42 = getelementptr inbounds nuw i8, ptr %41, i64 16
  store <4 x i32> %39, ptr %41, align 4, !tbaa !5
  store <4 x i32> %40, ptr %42, align 4, !tbaa !5
  %43 = add nuw i64 %34, 8
  %44 = icmp eq i64 %43, %32
  br i1 %44, label %45, label %33, !llvm.loop !68

45:                                               ; preds = %33
  %46 = icmp eq i64 %32, %8
  br i1 %46, label %72, label %47

47:                                               ; preds = %45, %25
  %48 = phi i64 [ 0, %25 ], [ %32, %45 ]
  %49 = and i64 %8, 3
  %50 = icmp eq i64 %49, 0
  br i1 %50, label %61, label %51

51:                                               ; preds = %51, %47
  %52 = phi i64 [ %58, %51 ], [ %48, %47 ]
  %53 = phi i64 [ %59, %51 ], [ 0, %47 ]
  %54 = getelementptr inbounds nuw i32, ptr %0, i64 %52
  %55 = load i32, ptr %54, align 4, !tbaa !5
  %56 = shl nsw i32 %55, 1
  %57 = getelementptr inbounds nuw i32, ptr %1, i64 %52
  store i32 %56, ptr %57, align 4, !tbaa !5
  %58 = add nuw nsw i64 %52, 1
  %59 = add i64 %53, 1
  %60 = icmp eq i64 %59, %49
  br i1 %60, label %61, label %51, !llvm.loop !69

61:                                               ; preds = %51, %47
  %62 = phi i64 [ %48, %47 ], [ %58, %51 ]
  %63 = sub nsw i64 %48, %8
  %64 = icmp ugt i64 %63, -4
  br i1 %64, label %72, label %73

65:                                               ; preds = %65, %23
  %66 = phi i64 [ %69, %65 ], [ %24, %23 ]
  %67 = getelementptr inbounds nuw i32, ptr %0, i64 %66
  %68 = trunc nuw nsw i64 %66 to i32
  store i32 %68, ptr %67, align 4, !tbaa !5
  %69 = add nuw nsw i64 %66, 1
  %70 = icmp eq i64 %69, %8
  br i1 %70, label %25, label %65, !llvm.loop !71

71:                                               ; preds = %3
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !66
  tail call void @codemarker_alpha(i32 noundef 28) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !67
  br label %72

72:                                               ; preds = %73, %71, %61, %45
  ret void

73:                                               ; preds = %73, %61
  %74 = phi i64 [ %94, %73 ], [ %62, %61 ]
  %75 = getelementptr inbounds nuw i32, ptr %0, i64 %74
  %76 = load i32, ptr %75, align 4, !tbaa !5
  %77 = shl nsw i32 %76, 1
  %78 = getelementptr inbounds nuw i32, ptr %1, i64 %74
  store i32 %77, ptr %78, align 4, !tbaa !5
  %79 = add nuw nsw i64 %74, 1
  %80 = getelementptr inbounds nuw i32, ptr %0, i64 %79
  %81 = load i32, ptr %80, align 4, !tbaa !5
  %82 = shl nsw i32 %81, 1
  %83 = getelementptr inbounds nuw i32, ptr %1, i64 %79
  store i32 %82, ptr %83, align 4, !tbaa !5
  %84 = add nuw nsw i64 %74, 2
  %85 = getelementptr inbounds nuw i32, ptr %0, i64 %84
  %86 = load i32, ptr %85, align 4, !tbaa !5
  %87 = shl nsw i32 %86, 1
  %88 = getelementptr inbounds nuw i32, ptr %1, i64 %84
  store i32 %87, ptr %88, align 4, !tbaa !5
  %89 = add nuw nsw i64 %74, 3
  %90 = getelementptr inbounds nuw i32, ptr %0, i64 %89
  %91 = load i32, ptr %90, align 4, !tbaa !5
  %92 = shl nsw i32 %91, 1
  %93 = getelementptr inbounds nuw i32, ptr %1, i64 %89
  store i32 %92, ptr %93, align 4, !tbaa !5
  %94 = add nuw nsw i64 %74, 4
  %95 = icmp eq i64 %94, %26
  br i1 %95, label %72, label %73, !llvm.loop !72
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @algebraic(i32 noundef %0) local_unnamed_addr #2 {
  ret i32 3
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @bit_tricks(i32 noundef %0) local_unnamed_addr #2 {
  %2 = mul i32 %0, 6
  ret i32 %2
}

; Function Attrs: nounwind uwtable
define dso_local void @empty_loop(i32 noundef %0) local_unnamed_addr #3 {
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !73
  tail call void @codemarker_alpha(i32 noundef 29) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !74
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #3 {
  %1 = alloca [32 x i8], align 16
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !75
  tail call void @codemarker_alpha(i32 noundef 30) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !76
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !77
  tail call void @codemarker_alpha(i32 noundef 31) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !78
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !10
  tail call void @codemarker_alpha(i32 noundef 0) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !11
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !79
  tail call void @codemarker_alpha(i32 noundef 32) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !80
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !17
  tail call void @codemarker_alpha(i32 noundef 1) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !18
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !19
  tail call void @codemarker_alpha(i32 noundef 2) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !20
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i32 noundef 27)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !81
  tail call void @codemarker_alpha(i32 noundef 33) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !82
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !21
  tail call void @codemarker_alpha(i32 noundef 3) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !22
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !23
  tail call void @codemarker_alpha(i32 noundef 4) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !24
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !25
  tail call void @codemarker_alpha(i32 noundef 5) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !26
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !27
  tail call void @codemarker_alpha(i32 noundef 6) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !28
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !29
  tail call void @codemarker_alpha(i32 noundef 7) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !30
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.1, i32 noundef 20)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !83
  tail call void @codemarker_alpha(i32 noundef 34) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !84
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !31
  tail call void @codemarker_alpha(i32 noundef 8) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !32
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !33
  tail call void @codemarker_alpha(i32 noundef 9) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !34
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !35
  tail call void @codemarker_alpha(i32 noundef 10) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !36
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.2, i32 noundef 48)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !85
  tail call void @codemarker_alpha(i32 noundef 35) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !86
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !37
  tail call void @codemarker_alpha(i32 noundef 11) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !38
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !39
  tail call void @codemarker_alpha(i32 noundef 12) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !40
  %5 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.3, i32 noundef 24)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !87
  tail call void @codemarker_alpha(i32 noundef 36) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !88
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.4, i32 noundef 120)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !89
  tail call void @codemarker_alpha(i32 noundef 37) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !90
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !55
  tail call void @codemarker_alpha(i32 noundef 19) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !56
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.5, i32 noundef 200)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !91
  tail call void @codemarker_alpha(i32 noundef 38) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !92
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !57
  tail call void @codemarker_alpha(i32 noundef 20) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !58
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.6, i32 noundef 66)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !93
  tail call void @codemarker_alpha(i32 noundef 39) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !94
  %9 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.7, i32 noundef 3)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !95
  tail call void @codemarker_alpha(i32 noundef 40) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !96
  %10 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.8, i32 noundef 42)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !97
  tail call void @codemarker_alpha(i32 noundef 41) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !98
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !99
  tail call void @codemarker_alpha(i32 noundef 42) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !100
  call void @llvm.lifetime.start.p0(i64 32, ptr nonnull %1) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !101
  tail call void @codemarker_alpha(i32 noundef 43) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !102
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !61
  tail call void @codemarker_alpha(i32 noundef 26) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !62
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 16 dereferenceable(32) %1, ptr noundef nonnull readonly align 16 dereferenceable(32) @__const.main.src, i64 32, i1 false)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !103
  tail call void @codemarker_alpha(i32 noundef 44) #7
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !104
  %11 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.9, ptr noundef nonnull %1)
  call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !105
  call void @codemarker_alpha(i32 noundef 45) #7
  call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !106
  call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !107
  call void @codemarker_alpha(i32 noundef 46) #7
  call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #7, !srcloc !108
  %12 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.10, i32 noundef 100, i32 noundef 25, i32 noundef 20)
  call void @llvm.lifetime.end.p0(i64 32, ptr nonnull %1) #7
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #5

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) #6

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.vector.reduce.add.v4i32(<4 x i32>) #6

attributes #0 = { noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #5 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #7 = { nounwind }

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
!9 = !{i64 27399}
!10 = !{i64 954}
!11 = !{i64 1015}
!12 = distinct !{!12, !13, !14, !15}
!13 = !{!"llvm.loop.mustprogress"}
!14 = !{!"llvm.loop.isvectorized", i32 1}
!15 = !{!"llvm.loop.unroll.runtime.disable"}
!16 = distinct !{!16, !13, !15, !14}
!17 = !{i64 1243}
!18 = !{i64 1304}
!19 = !{i64 1413}
!20 = !{i64 1474}
!21 = !{i64 1648}
!22 = !{i64 1709}
!23 = !{i64 1760}
!24 = !{i64 1821}
!25 = !{i64 1872}
!26 = !{i64 1933}
!27 = !{i64 1993}
!28 = !{i64 2054}
!29 = !{i64 2114}
!30 = !{i64 2175}
!31 = !{i64 2390}
!32 = !{i64 2451}
!33 = !{i64 2501}
!34 = !{i64 2562}
!35 = !{i64 2730}
!36 = !{i64 2792}
!37 = !{i64 2960}
!38 = !{i64 3022}
!39 = !{i64 3077}
!40 = !{i64 3139}
!41 = !{i64 3497}
!42 = !{i64 3559}
!43 = !{i64 3611}
!44 = !{i64 3673}
!45 = distinct !{!45, !13, !14, !15}
!46 = distinct !{!46, !13, !15, !14}
!47 = !{i64 4142}
!48 = !{i64 4204}
!49 = !{i64 4256}
!50 = !{i64 4318}
!51 = distinct !{!51, !13, !14, !15}
!52 = distinct !{!52, !13, !15, !14}
!53 = !{i64 4757}
!54 = !{i64 4819}
!55 = !{i64 4992}
!56 = !{i64 5054}
!57 = !{i64 5222}
!58 = !{i64 5284}
!59 = !{i64 5455}
!60 = !{i64 5517}
!61 = !{i64 6105}
!62 = !{i64 6167}
!63 = !{i64 6363}
!64 = !{i64 6425}
!65 = distinct !{!65, !13, !14, !15}
!66 = !{i64 6512}
!67 = !{i64 6574}
!68 = distinct !{!68, !13, !14, !15}
!69 = distinct !{!69, !70}
!70 = !{!"llvm.loop.unroll.disable"}
!71 = distinct !{!71, !13, !15, !14}
!72 = distinct !{!72, !13, !14}
!73 = !{i64 6964}
!74 = !{i64 7026}
!75 = !{i64 7227}
!76 = !{i64 7289}
!77 = !{i64 7349}
!78 = !{i64 7411}
!79 = !{i64 7480}
!80 = !{i64 7542}
!81 = !{i64 7642}
!82 = !{i64 7704}
!83 = !{i64 7794}
!84 = !{i64 7856}
!85 = !{i64 7958}
!86 = !{i64 8020}
!87 = !{i64 8113}
!88 = !{i64 8175}
!89 = !{i64 8282}
!90 = !{i64 8344}
!91 = !{i64 8429}
!92 = !{i64 8491}
!93 = !{i64 8569}
!94 = !{i64 8631}
!95 = !{i64 8717}
!96 = !{i64 8779}
!97 = !{i64 8866}
!98 = !{i64 8928}
!99 = !{i64 9010}
!100 = !{i64 9072}
!101 = !{i64 9125}
!102 = !{i64 9187}
!103 = !{i64 9249}
!104 = !{i64 9311}
!105 = !{i64 9380}
!106 = !{i64 9442}
!107 = !{i64 9501}
!108 = !{i64 9563}

^0 = module: (path: "./file4_origin_1_LLVMIR_LINK_PREOPT/file4_origin.bc", hash: (0, 0, 0, 0, 0))
^1 = gv: (name: "algebraic", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 1, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 736499631867688647
^2 = gv: (name: ".str.8", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 1530094498781262953
^3 = gv: (name: "copy_buffer", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 5, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 1966500351621579433
^4 = gv: (name: ".str.7", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 2259290126537346677
^5 = gv: (name: "llvm.memcpy.p0.p0.i64") ; guid = 2699563562842851132
^6 = gv: (name: "inline_candidate", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 3, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 2990092464636197017
^7 = gv: (name: "llvm.lifetime.start.p0") ; guid = 4736283418132453162
^8 = gv: (name: "bit_tricks", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 2, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 4887551538650224416
^9 = gv: (name: ".str.3", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 5633919400532139758
^10 = gv: (name: "llvm.smax.i32") ; guid = 6002034434922093760
^11 = gv: (name: "dead_code", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 2, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 6257192332597860212
^12 = gv: (name: ".str.9", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 6477107446441723275
^13 = gv: (name: "printf") ; guid = 7383291119112528047
^14 = gv: (name: "max", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 5, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 8185957744030711343
^15 = gv: (name: "cse_example", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 9, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 8453375148749674536
^16 = gv: (name: "multiply_by_two", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 2, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 8477806741152019488
^17 = gv: (name: "branch_elimination", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 10, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 8699142124250108312
^18 = gv: (name: ".str.5", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 9149013450228509752
^19 = gv: (name: "llvm.compiler.used", summaries: (variable: (module: ^0, flags: (linkage: appending, visibility: default, notEligibleToImport: 1, live: 1, dsoLocal: 0, canAutoHide: 0, importType: definition), varFlags: (readonly: 0, writeonly: 0, constant: 0), refs: (^31)))) ; guid = 9610627770985738006
^20 = gv: (name: ".str.2", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 9652801601657790934
^21 = gv: (name: "constant_fold", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 16, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 9767906317069561782
^22 = gv: (name: ".str.6", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 9973172625557952486
^23 = gv: (name: "__const.main.src", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 10560701773776491305
^24 = gv: (name: ".str", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 10615243208080064493
^25 = gv: (name: "init_array", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 36, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 11372478009472378006
^26 = gv: (name: ".str.1", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 12349168911816387147
^27 = gv: (name: ".str.10", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 12371651202119254851
^28 = gv: (name: "empty_loop", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 4, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 12714221607190294795
^29 = gv: (name: "loop_invariant", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 47, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 12987039436391562778
^30 = gv: (name: "llvm.lifetime.end.p0") ; guid = 13244677477227836793
^31 = gv: (name: "codemarker_alpha", summaries: (function: (module: ^0, flags: (linkage: internal, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 6, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 1, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0)))) ; guid = 13833711830623828848
^32 = gv: (name: "loop_fusion", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 112, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 14397824925514798483
^33 = gv: (name: "compute_something", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 9, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 15258918510218081799
^34 = gv: (name: "store_load", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 6, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 15261083965881353416
^35 = gv: (name: "main", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 115, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1), (callee: ^13, tail: 1)), refs: (^24, ^26, ^20, ^9, ^39, ^18, ^22, ^4, ^2, ^23, ^12, ^27)))) ; guid = 15822663052811949562
^36 = gv: (name: "llvm.vector.reduce.add.v4i32") ; guid = 16711401006643563310
^37 = gv: (name: "strength_reduce", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 40, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 17890632957303120142
^38 = gv: (name: "grade", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 15, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^31, tail: 1))))) ; guid = 18101706878112262878
^39 = gv: (name: ".str.4", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 18267160176698062867
^40 = flags: 8
^41 = blockcount: 0
