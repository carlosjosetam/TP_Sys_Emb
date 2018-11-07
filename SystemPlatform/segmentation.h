/*
 * SoCLib is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; version 2.1 of the License.
 * 
 * SoCLib is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with SoCLib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#define	MEMORY_BASE	0x10000000
#define	MEMORY_SIZE	0x02000000

#define	RESET_BASE	0xBFC00000
#define	RESET_SIZE	0x00010000

#define	EXCEP_BASE	0x80000000
#define	EXCEP_SIZE	0x00010000

#define CACHE_BASE  0xD0000000
#define CACHE_SIZE  0x00000100

#define LOG_BASE    0xC0000000
#define LOG_SIZE    0x00000004

#define	TTY_BASE	    0xC1000000
#define	TTY_SIZE	    0x00000040


#define	TIMER_BASE	  0xC2000000
#define	TIMER_SIZE	  0x00000100

#define	MAILBOX_BASE	0xC3000000
#define	MAILBOX_SIZE	0x00000400

#define	AICU_BASE	    0xC4000000
#define	AICU_SIZE	    0x00001000
