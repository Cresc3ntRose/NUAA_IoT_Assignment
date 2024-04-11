fn check_memory_attributes() {
    // 栈
    let stack_var: u32 = 42;
    println!("Stack memory is readable and writable, but not executable.");
    println!("Value of stack variable: {}", stack_var);

    // 堆
    let heap_var = Box::new(42u32);
    println!("Heap memory is readable and writable, but not executable.");
    println!("Value of heap variable: {}", *heap_var);

    // 数据区
    static DATA_VAR: u32 = 42;
    println!("Data segment is readable and writable, but not executable.");
    println!("Value of data variable: {}", DATA_VAR);

    // 代码段
    let code_segment_address = check_memory_attributes as *const fn();
    println!("Code segment is readable and executable, but not writable.");
    unsafe {
        println!("Value at code segment address: {:?}", code_segment_address);
    }
}

fn main() {
    check_memory_attributes();
}
