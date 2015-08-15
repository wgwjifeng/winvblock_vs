/**
 * Copyright (C) 2009-2011, Shao Miller <shao.miller@yrdsb.edu.on.ca>.
 * Copyright 2006-2008, V.
 * For WinAoE contact information, see http://winaoe.org/
 *
 * This file is part of WinVBlock, derived from WinAoE.
 *
 * WinVBlock is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WinVBlock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WinVBlock.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef WV_M_DEVICE_H_
#  define WV_M_DEVICE_H_

/**
 * @file
 *
 * Device specifics.
 */

typedef enum _WV_DEV_STATE {
    WvDevStateNotStarted,
    WvDevStateStarted,
    WvDevStateStopPending,
    WvDevStateStopped,
    WvDevStateRemovePending,
    WvDevStateSurpriseRemovePending,
    WvDevStateDeleted,
    WvDevStates
  } WV_E_DEV_STATE, * PWV_EP_DEV_STATE;

/* Forward declarations. */
typedef struct WV_DEV_T WV_S_DEV_T, * WV_SP_DEV_T;

/**
 * Device PDO creation routine.
 *
 * @v dev               The device whose PDO should be created.
 * @ret pdo             Points to the new PDO, or is NULL upon failure.
 */
typedef PDEVICE_OBJECT STDCALL WV_F_DEV_CREATE_PDO(IN WV_SP_DEV_T);
typedef WV_F_DEV_CREATE_PDO * WV_FP_DEV_CREATE_PDO;
extern WVL_M_LIB WV_F_DEV_CREATE_PDO WvDevCreatePdo;

/**
 * Device initialization routine.
 *
 * @v dev               The device being initialized.
 */
typedef BOOLEAN STDCALL WV_F_DEV_INIT(IN WV_SP_DEV_T);
typedef WV_F_DEV_INIT * WV_FP_DEV_INIT;

/**
 * Device PnP ID reponse routine.
 *
 * @v dev                       The device being queried for PnP IDs.
 * @v query_type                The query type.
 * @v buf                       Wide character, 512-element buffer for the
 *                              ID response.
 * @ret UINT32       The number of wide characters in the response.
 */
typedef UINT32 STDCALL WV_F_DEV_PNP_ID(
    IN WV_SP_DEV_T,
    IN BUS_QUERY_ID_TYPE,
    IN OUT WCHAR (*)[512]
  );
typedef WV_F_DEV_PNP_ID * WV_FP_DEV_PNP_ID;
extern WVL_M_LIB WV_F_DEV_PNP_ID WvDevPnpId;

/**
 * Device close routine.
 *
 * @v dev               The device being closed.
 */
typedef VOID STDCALL WV_F_DEV_CLOSE(IN WV_SP_DEV_T);
typedef WV_F_DEV_CLOSE * WV_FP_DEV_CLOSE;
extern WVL_M_LIB WV_F_DEV_CLOSE WvDevClose;

/**
 * Device deletion routine.
 *
 * @v dev_ptr           Points to the device to delete.
 */
typedef VOID STDCALL WV_F_DEV_FREE(IN WV_SP_DEV_T);
typedef WV_F_DEV_FREE * WV_FP_DEV_FREE;
extern WVL_M_LIB WV_F_DEV_FREE WvDevFree;

extern WVL_M_LIB VOID WvDevInit(WV_SP_DEV_T);
extern WVL_M_LIB WV_SP_DEV_T WvDevCreate(void);

typedef struct WV_DEV_OPS {
    WV_FP_DEV_CREATE_PDO CreatePdo;
    WV_FP_DEV_INIT Init;
    WV_FP_DEV_PNP_ID PnpId;
    WV_FP_DEV_CLOSE Close;
    WV_FP_DEV_FREE Free;
  } WV_S_DEV_OPS, * WV_SP_DEV_OPS;

/**
 * The prototype for a device IRP dispatch.
 *
 * @v dev               Points to the device.
 * @v irp               Points to the IRP.
 * @ret NTSTATUS        The status of processing the IRP for the device.
 */
typedef NTSTATUS STDCALL WV_F_DEV_DISPATCH(IN WV_SP_DEV_T, IN PIRP);
typedef WV_F_DEV_DISPATCH * WV_FP_DEV_DISPATCH;

/**
 * The prototype for a device IRP_MJ_DEVICE_CONTROL dispatch.
 *
 * @v dev               Points to the device.
 * @v irp               Points to the IRP.
 * @v code              The I/O control code.
 * @ret NTSTATUS        The status of processing the IRP for the device.
 */
typedef NTSTATUS STDCALL WV_F_DEV_CTL(
    IN WV_SP_DEV_T,
    IN PIRP,
    IN ULONG POINTER_ALIGNMENT
  );
typedef WV_F_DEV_CTL * WV_FP_DEV_CTL;

/**
 * The prototype for a device IRP_MJ_SCSI dispatch.
 *
 * @v dev               Points to the device.
 * @v irp               Points to the IRP.
 * @v code              The SCSI function.
 * @ret NTSTATUS        The status of processing the IRP for the device.
 */
typedef NTSTATUS STDCALL WV_F_DEV_SCSI(IN WV_SP_DEV_T, IN PIRP, IN UCHAR);
typedef WV_F_DEV_SCSI * WV_FP_DEV_SCSI;

/**
 * The prototype for a device IRP_MJ_PNP dispatch.
 *
 * @v dev               Points to the device.
 * @v irp               Points to the IRP.
 * @v code              The minor function.
 * @ret NTSTATUS        The status of processing the IRP for the device.
 */
typedef NTSTATUS STDCALL WV_F_DEV_PNP(IN WV_SP_DEV_T, IN PIRP, IN UCHAR);
typedef WV_F_DEV_PNP * WV_FP_DEV_PNP;

/* Details common to all devices this driver works with */
struct WV_DEV_T {
    /* For debugging */
    BOOLEAN IsBus;
    /* Self is self-explanatory. */
    PDEVICE_OBJECT Self;
    /* Points to the parent bus' DEVICE_OBJECT */
    PDEVICE_OBJECT Parent;
    /* Points to the driver. */
    PDRIVER_OBJECT DriverObject;
    /* Current state of the device. */
    WV_E_DEV_STATE State;
    /* Previous state of the device. */
    WV_E_DEV_STATE OldState;
    /* Support being a node on a bus. */
    WVL_S_BUS_NODE BusNode;
    /* The device operations. */
    WV_S_DEV_OPS Ops;
    /* Points to further extensions. */
    PVOID ext;
    /* Was the device established at boot time? */
    BOOLEAN Boot;
  };

extern WVL_M_LIB WV_SP_DEV_T WvDevFromDevObj(PDEVICE_OBJECT);
extern WVL_M_LIB VOID WvDevForDevObj(PDEVICE_OBJECT, WV_SP_DEV_T);
extern WVL_M_LIB PDRIVER_DISPATCH STDCALL WvDevGetIrpHandler(
    IN PDEVICE_OBJECT
  );
extern WVL_M_LIB VOID STDCALL WvDevSetIrpHandler(
    IN PDEVICE_OBJECT,
    IN PDRIVER_DISPATCH
  );
extern WV_F_DEV_DISPATCH WvDevPnpQueryId;

#endif  /* WV_M_DEVICE_H_ */
