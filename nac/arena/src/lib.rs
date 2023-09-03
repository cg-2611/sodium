use std::cell::{Cell, RefCell};

const CHUNK_SIZE: usize = 32;

pub struct TypedArena<T> {
    ptr: Cell<*mut T>,
    end: Cell<*mut T>,
    chunks: RefCell<Vec<TypedArenaChunk<T>>>,
}

impl<T> TypedArena<T> {
    pub fn new() -> Self {
        Self {
            ptr: Cell::new(std::ptr::null_mut()),
            end: Cell::new(std::ptr::null_mut()),
            chunks: RefCell::new(Vec::new()),
        }
    }

    pub fn alloc(&self, data: T) -> &T {
        if self.ptr == self.end {
            self.grow(1)
        }

        unsafe {
            self.ptr.set(self.ptr.get().add(1));
            std::ptr::write(self.ptr.get(), data);
            &*self.ptr.get()
        }
    }

    fn grow(&self, n: usize) {
        let size = std::cmp::max(1, std::mem::size_of::<T>());
        let mut chunks = self.chunks.borrow_mut();

        let capacity = std::cmp::max(n, CHUNK_SIZE / size);
        let mut chunk = TypedArenaChunk::<T>::new(capacity);

        self.ptr.set(chunk.start());
        self.end.set(chunk.end());

        chunks.push(chunk);
    }
}

impl<T> Default for TypedArena<T> {
    fn default() -> Self {
        Self::new()
    }
}

pub struct TypedArenaChunk<T> {
    storage: Vec<T>,
}

impl<T> TypedArenaChunk<T> {
    pub fn new(n: usize) -> Self {
        Self {
            storage: Vec::with_capacity(n),
        }
    }

    fn start(&mut self) -> *mut T {
        self.storage.as_ptr() as *mut T
    }

    fn end(&mut self) -> *mut T {
        unsafe { self.start().add(self.storage.len()) }
    }
}
