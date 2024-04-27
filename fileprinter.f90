SUBROUTINE load_lu() BIND(C, NAME='load_lu')
    open(10, access='stream', status='old', action='read', position='rewind')
END SUBROUTINE load_lu

! TODO: add subroutine to print file not parsed by logical unit
SUBROUTINE print_lu() BIND(C, NAME='print_lu')
    use, intrinsic :: iso_fortran_env, only: iostat_end
    implicit none
    
    integer   :: io_status
    character :: char

    ! https://stackoverflow.com/questions/5718283/is-there-a-simple-way-to-read-from-a-file-and-print-to-stdout-in-fortran
    do
        ! TODO: print location of logical unit?
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
