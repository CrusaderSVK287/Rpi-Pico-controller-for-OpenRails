#![no_std]
#![no_main]
use rp_pico::entry;
use panic_halt as _;
#[entry]
fn see_doesnt_have_to_be_called_main() -> ! {
  loop {}
}