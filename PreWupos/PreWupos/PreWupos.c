#include "PreWupos.h"

#define DEVICE_SEND_BUFF CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_DATA)
#define DEVICE_SEND_DIRECT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_IN_DIRECT, FILE_WRITE_DATA)
#define DEVICE_SEND_NEITHER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_NEITHER, FILE_WRITE_DATA)

#define DEVICE_REC_BUFF CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_READ_ACCESS)
#define DEVICE_REC_DIRECT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x805, METHOD_OUT_DIRECT, FILE_READ_DATA)
#define DEVICE_REC_NEITHER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x806, METHOD_NEITHER, FILE_READ_DATA)

UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\PreWuposDevice");
UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(L"\\??\\PreWuposLink");

PDEVICE_OBJECT DeviceObject = NULL;

NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT driverObject,
    _In_ PUNICODE_STRING registryPath
)
{
    DbgPrintEx(0, 0, ("[win-driver]"__FUNCTION__));
    UNREFERENCED_PARAMETER(registryPath);

    NTSTATUS status;

    driverObject->DriverUnload = DriverUnload;

    status = IoCreateDevice(driverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);

    if (!NT_SUCCESS(status)) {
        DbgPrintEx(0, 0, ("Creating device failed! \r\n"));
        return status;
    }

    status = IoCreateSymbolicLink(&SymbolicLinkName, &DeviceName);

    if (!NT_SUCCESS(status)) {
        DbgPrintEx(0, 0, ("Creating symbolic link failed! \r\n"));
        IoDeleteDevice(DeviceObject);

        return status;
    }

    for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; ++i) {
        driverObject->MajorFunction[i] = DispatchPassThru;
    }


    // driverObject->MajorFunction[IRP_MJ_READ] = DispatchCustom;
    // driverObject->MajorFunction[IRP_MJ_WRITE] = DispatchCustom;


    DbgPrintEx(0, 0, ("Driver has been loaded \r\n"));

    RTL_OSVERSIONINFOW operationSystemData;
    RtlGetVersion(&operationSystemData);
    // Print data about os
    DbgPrintEx(0, 0, "Data about system: windows: %u %u.%u",
        (unsigned int)operationSystemData.dwBuildNumber,
        (unsigned int)operationSystemData.dwMajorVersion,
        (unsigned int)operationSystemData.dwMinorVersion);

    return status;
}


VOID DriverUnload(
    _In_ PDRIVER_OBJECT driverObject)
{
    IoDeleteSymbolicLink(&SymbolicLinkName);
    IoDeleteDevice(DeviceObject);

    DbgPrintEx(0, 0, ("Driver has been unloaded \r\n"));
}

NTSTATUS DispatchPassThru(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS status = STATUS_SUCCESS;

    switch (irpsp->MajorFunction) {
    case IRP_MJ_CREATE:
        DbgPrintEx(0, 0, ("create request \r\n"));
        break;
    case IRP_MJ_CLOSE:
        DbgPrintEx(0, 0, ("create request \r\n"));
        break;
    case IRP_MJ_READ:
        DbgPrintEx(0, 0, ("create request \r\n"));
        break;
    default:
        break;
    }

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return status;
}


