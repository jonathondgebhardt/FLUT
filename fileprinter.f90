! https://www.ibm.com/docs/en/xffbg/121.141?topic=attributes-open
SUBROUTINE load_lu() BIND(C, NAME='load_lu')
    ! Unit:
    ! is a unit specifier in which u must be an external unit identifier whose value is not an asterisk. An external unit identifier 
    ! refers to an external file that is represented by an integer expression. The integer expression has one of the following values:
    ! - A value in the range 1 through 2147483647
    ! - Fortran 2008: A NEWUNIT value
    ! 
    ! If the optional characters UNIT= are omitted, u must be the first item in open_list.
    !
    ! Access:
    ! specifies the access method for the connection of the file. char_expr is a scalar character expression whose value, when 
    ! any trailing blanks are removed, is either SEQUENTIAL, DIRECT or STREAM. If ACCESS= is DIRECT, RECL= must be specified. 
    ! If ACCESS= is STREAM, RECL= must not be specified.
    !
    ! SEQUENTIAL is the default, for which RECL= is optional
    !
    ! Status:
    ! specifies the status of the file when it is opened. char_expr is a scalar character expression whose value, when any trailing 
    ! blanks are removed, is one of the following:
    ! - OLD, to connect an existing file to a unit. If OLD is specified, the file must exist. If the file does not exist, an error 
    ! condition will occur.
    ! - NEW, to create a new file, connect it to a unit, and change the status to OLD. If NEW is specified, the file must not exist. 
    ! If the file already exists, an error condition will occur.
    ! - SCRATCH, to create and connect a new file that will be deleted when it is disconnected. SCRATCH must not be specified with a
    ! named file (that is, FILE=char_expr must be omitted).
    ! - REPLACE. If the file does not already exist, the file is created and the status is changed to OLD. If the file exists, the 
    ! file is deleted, a new file is created with the same name, and the status is changed to OLD.
    ! - UNKNOWN, to connect an existing file, or to create and connect a new file. If the file exists, it is connected as OLD. If
    ! the file does not exist, it is connected as NEW. This is the default.
    !
    ! Position:
    ! specifies the file position for a file connected for sequential or stream access. A file that did not exist previously is 
    ! positioned at its initial point. char_expr is a scalar character expression whose value, when any trailing blanks are removed,
    ! is either ASIS, REWIND, or APPEND. REWIND positions the file at its initial point. APPEND positions the file before the 
    ! endfile record or, if there is no endfile record, at the terminal point. ASIS leaves the position unchanged. The default 
    ! value is ASIS except under the following conditions
    open(10, access='stream', status='old', action='read', position='rewind')
END SUBROUTINE load_lu

! https://stackoverflow.com/questions/5718283/is-there-a-simple-way-to-read-from-a-file-and-print-to-stdout-in-fortran
SUBROUTINE print_lu() BIND(C, NAME='print_lu')
    use, intrinsic :: iso_fortran_env, only: iostat_end
    implicit none
    
    integer   :: io_status, openedq
    character :: char
    character(len=64) :: name_of_file

    ! Not very interesting, just shows relative file path.
    ! inquire(unit=10, name=name_of_file)
    ! write(*,*) name_of_file

    do
        read(10, iostat=io_status) char
        if(io_status == iostat_end) exit
        if(io_status > 0) error stop '*** Error occurred while reading file. ***'
        write(*, '(a)', advance='no') char
    end do
END SUBROUTINE print_lu

SUBROUTINE close_lu() BIND(C, NAME='close_lu')
    close(10)
END SUBROUTINE close_lu

SUBROUTINE load_file() BIND(C, NAME='load_file')
    open(10, access='stream', status='old', action='read')
END SUBROUTINE load_file

! https://stackoverflow.com/questions/5718283/is-there-a-simple-way-to-read-from-a-file-and-print-to-stdout-in-fortran
SUBROUTINE print_file() BIND(C, NAME='print_file')
    use, intrinsic :: iso_fortran_env, only: iostat_end
    implicit none

    character(*), parameter :: file_name = 'file.txt'

    integer   :: lun, io_status
    character :: char

    open(newunit=lun, file=file_name, access='stream', status='old',  &
         action='read')

    do
        read(lun, iostat=io_status) char
        if(io_status == iostat_end) exit
        if(io_status > 0) error stop '*** Error occurred while reading file. ***'
        write(*, '(a)', advance='no') char
    end do

    close(lun)
END SUBROUTINE print_file

SUBROUTINE close_file() BIND(C, NAME='close_file')
    close(10)
END SUBROUTINE close_file
