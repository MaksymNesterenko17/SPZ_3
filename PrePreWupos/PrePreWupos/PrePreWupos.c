#include "PrePreWupos.h"

#define DEVICE_SEND_BUFF CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_DATA)
#define DEVICE_SEND_DIRECT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_IN_DIRECT, FILE_WRITE_DATA)
#define DEVICE_SEND_NEITHER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_NEITHER, FILE_WRITE_DATA)

#define DEVICE_REC_BUFF CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_READ_ACCESS)
#define DEVICE_REC_DIRECT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x805, METHOD_OUT_DIRECT, FILE_READ_DATA)
#define DEVICE_REC_NEITHER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x806, METHOD_NEITHER, FILE_READ_DATA)

UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\mydevicedriver");
UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(L"\\??\\mydevicedriverlink");

PDEVICE_OBJECT DeviceObject = NULL;

NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT driverObject,
    _In_ PUNICODE_STRING registryPath
)
{
    DbgPrintEx(0, 0, ("[win-driver]"__FUNCTION__));
    UNREFERENCED_PARAMETER(registryPath);

    NTSTATUS status;


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




