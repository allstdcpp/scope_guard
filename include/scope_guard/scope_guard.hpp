
#ifndef SCOPE_GUARD_SGUARD
#define SCOPE_GUARD_SGUARD

#include <functional>
#include <utility>

namespace allstdcpp {
    namespace guard {

        template<typename T>
        class scope_guard {
        public:
            using function_t = std::function<void(T&)>;

            scope_guard(T& object, function_t function)
                : object_(&object),
                  function_(std::move(function)) {
            }

            scope_guard(scope_guard const&) = default;
            scope_guard& operator=(scope_guard const&) = default;

            scope_guard(scope_guard&& rhs) noexcept
                : object_ {rhs.object_},
                  function_ {std::move(rhs.function_)},
                  destruct_ {rhs.destruct_} {
                rhs.object_ = nullptr;
                rhs.destruct_ = false;
            }

            scope_guard& operator=(scope_guard&& rhs) noexcept {
                if(&rhs != this) {
                    destroy();
                    this->object_ = rhs.object_;
                    this->function_ = std::move(rhs.function_);
                    this->destruct_ = rhs.destruct_;
                    rhs.destruct_ = false;
                }
                return *this;
            }

            function_t deleter() const {
                return function_;
            }

            void destroy() noexcept {
                try {
                    if(destruct_) function_(*object_);
                } catch(...) {}
                destruct_ = false;
            }

            void release() noexcept {
                destruct_ = false;
            }

            ~scope_guard() {
                destroy();
            }

        private:
            T* object_ {};
            function_t function_ {};
            bool destruct_ {true};
        };

        template<typename T>
        scope_guard<T> create_scope_guard_for(T& object, std::function<void(T&)> cleanup) {
            return scope_guard<T> {object, std::move(cleanup)};
        }

    }

}

#endif
